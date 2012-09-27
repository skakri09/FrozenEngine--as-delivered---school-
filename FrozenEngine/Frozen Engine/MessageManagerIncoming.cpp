#include "MessageManagerIncoming.h"
#include "xmlSettingsHandling.h"
/*  P U B L  I C    F U N C T I O N S  */
/*-------------------------------------*/
CIncomingMessageManager::CIncomingMessageManager() : log("IncomingMsgMan", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	eCurrentHeldPackage = EMPTY;
	incompleteMessage = false;
	_sizeLeftOfMissingMsg = 0;
}

CIncomingMessageManager::~CIncomingMessageManager()
{
	FinishedReading(true);
}

void CIncomingMessageManager::LoadMessage(char* sourceBuffer, int messageSize) 
{	
	if( (messageSize > 0) && (messageSize <= frozen::maxNetBufSize) )
	{
		int endOfLastMessage = 0; // Bytes into the msg where end of last packet was and next will start

		if(incompleteMessage)
		{
			if(messageSize >= _sizeLeftOfMissingMsg)
			{
				log << INFO << "Attempting to finish incomplete msg from last receive" << endl;

				memcpy(_messageBuffer, sourceBuffer, _sizeLeftOfMissingMsg);
				_messages.push_back(MsgTuple(eCurrentHeldPackage, _messageBuffer, _sizeLeftOfMissingMsg+unhandledDataSize));
				endOfLastMessage += _sizeLeftOfMissingMsg;
				unhandledDataSize = messageSize - _sizeLeftOfMissingMsg;
				incompleteMessage = false;
			}
		}
		else
		{
			unhandledDataSize = messageSize; 
		}
		if(unhandledDataSize > 0)
		{
			bool moreMessages = true;
			while(moreMessages)
			{
				PackageType testType;
				memcpy(&testType, sourceBuffer+endOfLastMessage, frozen::PackageTypeSize);
				
				// Makes sure the next package is a real package, if so we keep going
				if(_VertifyPackageTypeExists(testType))
				{
					eCurrentHeldPackage = testType;

					int lastMsgSize = _FindMsgAndAddToVector(sourceBuffer+endOfLastMessage, testType);
					endOfLastMessage += lastMsgSize;
					if(!incompleteMessage){unhandledDataSize -= lastMsgSize;}
					if( (messageSize - endOfLastMessage) <= frozen::PackageTypeSize )
					{
						moreMessages = false;
					}
				}
				else
				{
					log << ERRORX << "The PackageType does not exist. Dismissing message" << endl;
					moreMessages = false;
				}	
			}
		}
	}
	else
	{
		// Error/disconnection handling
		_LoadMessageErrorHandling(&messageSize);
	}
	
}

bool CIncomingMessageManager::UnloadMessage(int msgNumber, void* target, int targetCapacity)
{	
	int messagesSize = _messages.size();
	bool returnVal = false;
	if(messagesSize >= msgNumber)
	{
		int msgSize = _messages.at(msgNumber).get<2>();
		if(msgSize == targetCapacity)
		{
			memcpy(target, _messages.at(msgNumber).get<1>(), targetCapacity);
			returnVal = true;
		}
		else
		{
			log << ERRORX << "messageSize != targetCapacity - Not unloading" << endl;
			returnVal =  false;
		}
		if(msgNumber == _messages.size()-1)
		{
			if(incompleteMessage){FinishedReading(false);}
			else{FinishedReading(true);}	
		}
	}
	else
	{
		log << ERRORX << "message does not exist in _messages vector" << endl;
		return false;
	}
	return returnVal;
}



/*  P R I V A T E    F U N C T I O N S  */
/*--------------------------------------*/

void CIncomingMessageManager::_LoadMessageErrorHandling( int* msgSize )
{
	// MessageSize was 0, means the other end disconnected. We flag the socket for closing
	if(*msgSize == 0)
	{
		log << INFO << "Client disconnected, flagging for socket close" << endl;
		eCurrentHeldPackage = DISCONNECT;
	}

	// Errors, logging error msg and flagging socket for closing
	else if(*msgSize == -1)
	{
		log << ERRORX << "ERROR, size -1, closing socket" << endl;
		eCurrentHeldPackage = DISCONNECT;
	}
	else if(*msgSize > frozen::maxNetBufSize)
	{
		log << ERRORX << "messageSize > maxNetBufSize - not loading the message!" << endl;
		eCurrentHeldPackage = EMPTY;
	}
	else
	{
		log << ERRORX << "Unknown error (<-1), closing socket" << endl; 
		eCurrentHeldPackage = DISCONNECT;
	}
}
int CIncomingMessageManager::_HandleMsgType(int msgSize, char* sourceBuf)
{
	//int returnVal = *msgSize;
	_messageBuffer = new char[msgSize];
	_CtrlAndPlaceMsgInCharBuf(sourceBuf, msgSize);
	return msgSize;
}
int CIncomingMessageManager::_FindMsgAndAddToVector( char* sourceBuffer, PackageType type )
{
	int returnVal = 0;
	
	switch(eCurrentHeldPackage)
	{
	case NEW_PLAYER			 :	returnVal =	_HandleMsgType(frozen::newPlayerBaseInfoSize, sourceBuffer);			break;
	case NEW_NPC			 :	returnVal =	_HandleMsgType(frozen::newGameNpcSize, sourceBuffer);					break;
	case MOVEMENT_UPDATE	 :	returnVal = _HandleMsgType(frozen::movementUpdateSize, sourceBuffer);				break;
	case POSITION_CORRECTION :	returnVal = _HandleMsgType(frozen::characterPositionCorrectionSize, sourceBuffer);  break;
	case LAUNCHING_ATTACH	 :	returnVal = _HandleMsgType(frozen::launchingAttackSize, sourceBuffer);				break;
	case HEALTH_CORRECTION	 :	returnVal = _HandleMsgType(frozen::hpCorrectionSize, sourceBuffer);					break;
	default: log << ERRORX << "_FindMsgAndAddToVector() - PackageType matches no case." << endl;					break;
	}
	if(!incompleteMessage)
	{
		_messages.push_back(MsgTuple(eCurrentHeldPackage, _messageBuffer, returnVal));
	}
	else
	{
		returnVal = unhandledDataSize;
	}
	return returnVal;
}
	
void CIncomingMessageManager::_CtrlAndPlaceMsgInCharBuf( char* sourceBuffer, int tarSize )
{
	// If the amount of unhandled data >= tarSize we have a complete message and memcpy it.
	if(unhandledDataSize >= tarSize)
	{
		memcpy(_messageBuffer, sourceBuffer, tarSize);
	}
	// message is smaller then the size of its message type, means 
	// the message is incomplete. Setting incompleteMessage bool to true.
	else
	{
		log << INFO << "Remaining message size < target buffer. Placing in leftoverBuffer" << endl;
		memcpy(_messageBuffer, sourceBuffer, unhandledDataSize);
		_sizeLeftOfMissingMsg = tarSize-unhandledDataSize;
		incompleteMessage = true;
	}
}

bool CIncomingMessageManager::_VertifyPackageTypeExists( PackageType& packType )
{

	for(int i = 0; i <= frozen::elemsInPackageType; ++i)
	{
		if(packType == i){return true;}
	}
	return false;
}

void CIncomingMessageManager::_ClearMessagesVector()
{
	// Resetting and freeing dynamic memory in the _messages vector.
	for(auto i = _messages.begin(); i != _messages.end(); i++)
	{
		if((*i).get<1>())
		{
			delete[] (*i).get<1>();
			(*i).get<1>() = 0;
		}
	}
	// Clearing the _messages vector.
	_messages.clear();
}

void CIncomingMessageManager::FinishedReading(bool completeReset)
{
	if(completeReset)
	{
		_ClearMessagesVector();
		unhandledDataSize = 0;
		incompleteMessage = false;
		eCurrentHeldPackage = EMPTY;
		_sizeLeftOfMissingMsg = 0;
	}
	else
	{
		_ClearMessagesVector();
	}
}




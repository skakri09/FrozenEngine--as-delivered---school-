#include "MessageManagerOutgoing.h"
#include "xmlSettingsHandling.h"

COutgoingMessageManager::COutgoingMessageManager() : log("OutgoingMsgMan", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	_ResetBuffAndVars();
}

COutgoingMessageManager::~COutgoingMessageManager()
{
}

void COutgoingMessageManager::LoadMessage(PackageType ePackageType, void* messageStart, int messageSize)
{
	if( (sizeOfAllMessages + messageSize) <= frozen::maxNetBufSize)
	{
		memcpy(messageBuffer+sizeOfAllMessages, messageStart, messageSize);
		sizeOfAllMessages += messageSize;
		eCurrentHeldPackage = ePackageType;
	}
	else
	{
		log << ERRORX << "theMessageSize + messageSize > maxNetBufSize - increse maxNetBufSize" << endl;
	}
}

int COutgoingMessageManager::UnloadMessage(char* targetBuffer, int targetCapacity)
{
	if( (sizeOfAllMessages > 0) && (targetCapacity >= sizeOfAllMessages))
	{
		memcpy(targetBuffer, messageBuffer, sizeOfAllMessages);
	}
	else if(sizeOfAllMessages <= 0)
	{
		log << WARN << "theMessageSize <= 0 - message not unloaded" << endl;
	}
	else
	{
		log << WARN << "theMessageSize > targetCapacity - not unloaded, increse maxNetBufSize" << endl;
	}
	int returnVal = sizeOfAllMessages;
	_ResetBuffAndVars();
	return returnVal;
}



void COutgoingMessageManager::_ResetBuffAndVars()
{
	for(int i = 0; i < frozen::maxNetBufSize; i++)
	{
		messageBuffer[i] = 0;
	}
	eCurrentHeldPackage = EMPTY;
	sizeOfAllMessages = 0;
}

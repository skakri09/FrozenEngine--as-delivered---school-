#include "NetManager.h"

CNetManager::CNetManager(Uint16 port) : log("NetManager", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	if(SDLNet_Init() < 0)
	{
		log << ERRORX << " SDLNet_Init: " << SDLNet_GetError() << endl;	
	}
	_port = port;
	_isHost = true;
	_outMsg = NULL;
	_inMsg = NULL;
	_host = NULL;
	_client = NULL;
	localEnemiesVectorRef = NULL;
	localPlayerCharactersVectorRef = NULL;
	
}

CNetManager::CNetManager( std::string ipAddr, Uint16 port ) : log("NetManager", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	if(SDLNet_Init() < 0)
	{
		log << ERRORX << " SDLNet_Init: " << SDLNet_GetError() << endl;	
	}
	_port = port;
	_ipAddr = ipAddr;
	_isHost = false;
	_outMsg = NULL;
	_inMsg = NULL;
	_host = NULL;
	_client = NULL;
	localEnemiesVectorRef = NULL;
	localPlayerCharactersVectorRef = NULL;
}

CNetManager::~CNetManager()
{
	SDLNet_Quit();
}

bool CNetManager::Start()
{
	if(!_outMsg)
	{
		_outMsg = new COutgoingMessageManager();
	}
	if(!_inMsg)
	{
		_inMsg = new CIncomingMessageManager();
	}
	_newMsgToSend = false;
	if(_isHost)
	{
		if(!_host)
		{
			_host = new CHostSocket(_port);
		}
		if(!_client)
		{
			_client = new CClientSocket();
		}
	}
	else
	{
		_client = new CClientSocket(_ipAddr, _port);
		if(Connected())
		{
			SendInitialLocalCharactersInfo();
		}
	}
	
	return true;
}

void CNetManager::Stop()
{
	FreeAllocatedMemory();
}

void CNetManager::Update()
{
	// If client is not yet made or has been deleted because of a DC, 
	// we make a new one, ready to receive a connection
	if(!_client)
	{
		_client = new CClientSocket();
	}

	// If hosting, we check for connecting client(s), accepting if any.
	if(_isHost)
	{
		if(_host->Accept(*_client))
		{   /* Sending info about our local characters to the client */
			SendInitialLocalCharactersInfo();
		}
	}
	
	// If there is any activity in a connected client, we receive the message.
	if(_client->SockRdyToBeRead())
	{
		RecieveMessage();
	}

	UpdateAllRemoteCharacters();
	SendLocalCharacterUpdates();

	// If we have any new messages to send, we send them
	if(_newMsgToSend)
	{
		if(Connected())
		{
			_client->Send(*_outMsg);
			_newMsgToSend = false;
		}
		else{log << WARN << "Trying to send a msg without being connected to a client" << endl; _newMsgToSend = false;}
	}
}

bool CNetManager::Connected()
{
	if(_client)
	{
		if(_client->SocketEstablished())
		{
			return true;;
		}
	}
	return false;
}

void CNetManager::SetLocalPlayersVectorRef(std::vector<CCharacterObject*>* playersVectorReference)
{
	localPlayerCharactersVectorRef = playersVectorReference;
}

void CNetManager::SetLocalEnemiesVectorRef( std::vector<CEnemyObject*>* enemiesVectorReference )
{
	localEnemiesVectorRef = enemiesVectorReference;
}


/* P R I V A T E     F U N C T I O N S */
/*-------------------------------------*/

void CNetManager::FreeAllocatedMemory()
{
	_CleanupClient();

	if(_host)
	{
		delete _host;
		_host = NULL;
	}

	if(_outMsg)
	{
		delete _outMsg;
		_outMsg = NULL;
	}
	if(_inMsg)
	{
		delete _inMsg;
		_inMsg = NULL;
	}
	localEnemiesVectorRef = NULL;
	localPlayerCharactersVectorRef = NULL;	
}

void CNetManager::_CleanupClient()
{
	if(_client)
	{
		Uint32 otherIPAddr;
		bool wasConnected = false;
		if(Connected())
		{
			otherIPAddr = _client->getIpAddress().GetHostAddr();
			wasConnected = true;
		}

		delete _client;
		_client = NULL;

		if(wasConnected)
		{
			log << WARN << "Socket with  "<< SDLNet_Read32(&otherIPAddr) << " closed" << endl;
		}
	}
	if(!remoteCharactersMap.empty())
	{
		std::map<std::string, CCharacterObject*>::iterator iter;
		
		for(iter = remoteCharactersMap.begin(); iter != remoteCharactersMap.end(); iter++)
		{
			(*iter).second->FlagImageForRemoval();
			
			/* Todo: delete the images, but must be done AFTER */
			/* the image is removed from the renderlist		   */
			/*if((*iter).second)
			{
				delete (*iter).second;
				(*iter).second = 0;
			}*/
		}
		
	}
}

void CNetManager::RecieveMessage()
{
	if(!_inMsg)
	{
		_inMsg = new CIncomingMessageManager();
	}
	_client->Recieve(*_inMsg);

	// Fetching the last package handled by messageManager
	PackageType recvPackType = _inMsg->GetTheMsgType();
	// If that package was a disconnect, we close the socket.
	if(recvPackType==DISCONNECT)
	{
		_CleanupClient();
	}
	else
	{	// Reading one by one message from the incomingMessageManager
		int inMessageSize = _inMsg->GetMessages()->size();
		for(int i = 0; i < inMessageSize; i++)
		{
			log << INFO << "Msg nr: " << i+1 << endl;
			recvPackType = _inMsg->GetMessages()->at(i).get<0>();
			switch(recvPackType)
			{
			case DISCONNECT				:	_CleanupClient();			break;
			case EMPTY					:
				log << WARN << "Incomplete or empty package. No action taken" << endl;
				break;
			case NEW_PLAYER				:	_UnloadNewPlayerInfo(&i);	break;
			case NEW_NPC				:	_UnloadNewGameNpcInfo(&i);	break;
			case MOVEMENT_UPDATE		:	_UnloadMovementUpdate(&i);	break;
			case POSITION_CORRECTION	:	_UnloadPosCorrection(&i);	break;
			case LAUNCHING_ATTACH		:	_UnloadLaunchedAttack(&i);	break;
			case HEALTH_CORRECTION		:	_UnloadHpCorrection(&i);	break;
			}
		}
	}
}

void CNetManager::_UnloadNewPlayerInfo(int* msgNr)
{
	_newPlayer = new NewPlayerBaseInfo;
	_inMsg->UnloadMessage(*msgNr, _newPlayer, frozen::newPlayerBaseInfoSize);
	log << INFO << "NewPLayer id       :   " << _newPlayer->characterID << endl;
	log << INFO << "NewPLayer name     :   " << _newPlayer->playerName << endl;
	log << INFO << "NewPLayer modelName:   " << _newPlayer->modelName << endl;
	log << INFO << "NewPLayer xpos     :   " << _newPlayer->startXPos << endl;
	log << INFO << "NewPLayer ypos     :   " << _newPlayer->startYPos << endl;
	AddNewCharacter(_newPlayer);
}

void CNetManager::_UnloadNewGameNpcInfo(int* msgNr)
{
	_inMsg->UnloadMessage(*msgNr, &_newGameNPC, frozen::newGameNpcSize);
	log << INFO << "NewNPC id: " << _newGameNPC.characterID << endl;
	log << INFO << "NewNPC modelName: " << _newGameNPC.modelName << endl;
	log << INFO << "NewNPC xpos: " << _newGameNPC.startXPos << endl;
	log << INFO << "NewNPC ypos: " << _newGameNPC.startYPos << endl;
}

void CNetManager::_UnloadMovementUpdate(int* msgNr)
{
	_inMsg->UnloadMessage(*msgNr, &_movementUpdate, frozen::movementUpdateSize);
	log << INFO << "MovementUpdate id: " << _movementUpdate.characterID << endl;
	log << INFO << "MovementUpdate xpos: " << _movementUpdate.xPos << endl;
	log << INFO << "MovementUpdate ypos: " << _movementUpdate.yPos << endl;
	log << INFO << "MovementUpdate xvel: " << _movementUpdate.xVel << endl;
	log << INFO << "MovementUpdate yvel: " << _movementUpdate.yVel << endl;

	UpdateMovement(_movementUpdate.characterID);
}

void CNetManager::_UnloadPosCorrection(int* msgNr)
{
	_inMsg->UnloadMessage(*msgNr, &_PosCorrection, frozen::characterPositionCorrectionSize);
	log << INFO << "posCorrection id: " << _PosCorrection.characterID << endl;
	log << INFO << "posCorrection xpos: " << _PosCorrection.xPos << endl;
	log << INFO << "posCorrection ypos: " << _PosCorrection.yPos << endl;
}

void CNetManager::_UnloadLaunchedAttack(int* msgNr)
{
	_inMsg->UnloadMessage(*msgNr, &_launchedAttack, frozen::launchingAttackSize);
	log << INFO << "attack playerID: " << _launchedAttack.characterID << endl;
	log << INFO << "attack targetID: " << _launchedAttack.targetID << endl;
	log << INFO << "attack id: " << _launchedAttack.attackID << endl;
}

void CNetManager::_UnloadHpCorrection(int* msgNr)
{
	_inMsg->UnloadMessage(*msgNr, &_hpCorrection, frozen::hpCorrectionSize);
	log << INFO << "hp correction playerID: " << _hpCorrection.characterID << endl;
	log << INFO << "hp correction targetID: " << _hpCorrection.targetID << endl;
	log << INFO << "hp correction hpCorr: " << _hpCorrection.healthCorrection << endl;
}

void CNetManager::AddNewCharacter( NewPlayerBaseInfo* newPlayerInfo)
{
	CCharacterObject* newCharacter = new CCharacterObject(newPlayerInfo->modelName, newPlayerInfo->playerName);
	newCharacter->setPositionX( newPlayerInfo->startXPos );
	newCharacter->setPositionY( newPlayerInfo->startYPos );
	newCharacter->SetImageID(newPlayerInfo->characterID);

	remoteCharactersMap[newPlayerInfo->playerName] = newCharacter;
	remoteCharacterIDMap[newPlayerInfo->characterID] = newPlayerInfo->playerName;
	//CRenderEngine::Inst()->AddRenderImg(newCharacter);
}

void CNetManager::UpdateMovement( int characterID )
{
	if( remoteCharacterIDMap.find(characterID) != remoteCharacterIDMap.end() )
	{
		std::string charName = remoteCharacterIDMap.find(characterID)->second;
		
		if( remoteCharactersMap.find(charName) != remoteCharactersMap.end() )
		{
			CCharacterObject* charToAlter = remoteCharactersMap.find(charName)->second;//->setPositionX();
			charToAlter->setPositionX(_movementUpdate.xPos);
			charToAlter->setPositionY(_movementUpdate.yPos);
			charToAlter->setVelocityX(_movementUpdate.xVel);
			charToAlter->setVelocityY(_movementUpdate.yVel);
		}else{log << WARN << "Character name " << charName << " could not be found in remoteCharactersMap." << endl;}
	}else{log << WARN << "Character ID: " << characterID << " could not be found in remoteCharacterIDMap." << endl;}
}

void CNetManager::UpdateAllRemoteCharacters() 
{
	float deltaTicks = CTimer::Inst()->getDeltaTime();
	
	std::map<std::string, CCharacterObject*>::iterator iter;
	for(iter = remoteCharactersMap.begin(); iter != remoteCharactersMap.end(); iter++)
	{
		(*iter).second->setPositionX((*iter).second->getPositionX() + (*iter).second->getVelocityX() * deltaTicks);
	}
}

void CNetManager::SendLocalCharacterUpdates()
{
	if(Connected())
	{
		std::vector<CCharacterObject*>::iterator playerIter;
		for(playerIter = localPlayerCharactersVectorRef->begin(); playerIter != localPlayerCharactersVectorRef->end(); playerIter++)
		{
			if((*playerIter)->IsMovementUpdate())
			{
				SendCharacterMovement(*(*playerIter));
			}
		}
		if(IsHost())
		{
			std::vector<CEnemyObject*>::iterator enemyIter;
			for(enemyIter = localEnemiesVectorRef->begin(); enemyIter != localEnemiesVectorRef->end(); enemyIter++)
			{
				SendCharacterMovement(*(*enemyIter));
			}
		}
	}
}

void CNetManager::SendInitialLocalCharactersInfo()
{
	std::vector<CCharacterObject*>::iterator playerIter;
	for(playerIter = localPlayerCharactersVectorRef->begin(); playerIter != localPlayerCharactersVectorRef->end(); playerIter++)
	{
		SendNewCharacterInfo(*(*playerIter));
	}


	std::vector<CEnemyObject*>::iterator enemyIter;
	for(enemyIter = localEnemiesVectorRef->begin(); enemyIter != localEnemiesVectorRef->end(); enemyIter++)
	{
		SendNewEnemyInfo(*(*enemyIter));
	}
}

void CNetManager::SendCharacterMovement( CCharacterObject& charToSendInfoFrom )
{
	MovementUpdate moveUpdate; // Creating a message object for movement update

	// Setting the values we are sending
	moveUpdate.xPos = charToSendInfoFrom.getPositionX();
	moveUpdate.yPos = charToSendInfoFrom.getPositionY();
	moveUpdate.xVel = charToSendInfoFrom.getVelocityX();
	moveUpdate.yVel = charToSendInfoFrom.getVelocityY();
	moveUpdate.ePackageType = MOVEMENT_UPDATE;
	moveUpdate.characterID = charToSendInfoFrom.GetImageID();
	// Loading the message into the outgoing message manager
	_outMsg->LoadMessage(MOVEMENT_UPDATE, &moveUpdate, frozen::movementUpdateSize);
	_newMsgToSend = true;
}

void CNetManager::SendCharacterMovement( CEnemyObject& charToSendInfoFrom )
{
	MovementUpdate moveUpdate; // Creating a message object for movement update

	// Setting the values we are sending
	moveUpdate.xPos = charToSendInfoFrom.getPositionX();
	moveUpdate.yPos = charToSendInfoFrom.getPositionY();
	moveUpdate.xVel = charToSendInfoFrom.getVelocityX();
	moveUpdate.yVel = charToSendInfoFrom.getVelocityY();
	moveUpdate.ePackageType = MOVEMENT_UPDATE;
	moveUpdate.characterID = charToSendInfoFrom.GetImageID();
	// Loading the message into the outgoing message manager
	_outMsg->LoadMessage(MOVEMENT_UPDATE, &moveUpdate, frozen::movementUpdateSize);
	_newMsgToSend = true;
}

bool CNetManager::SendNewCharacterInfo( CCharacterObject& charToSendInfoFrom )
{
	NewPlayerBaseInfo newChar; // Creating a message object for new player info

	// Copying the model and player name into our message object, returning false if it fails.
	const char* model = charToSendInfoFrom.GetSurfaceName().c_str();
	if(sizeof(newChar.modelName) >= strlen(model)+1)
	{
		memcpy(&newChar.modelName, model, strlen(model));
		newChar.modelName[strlen(model)] = '\0';

		const char* playerName = charToSendInfoFrom.GetPlayerName().c_str();
		if(sizeof(newChar.playerName) >= strlen(playerName)+1)
		{
			memcpy(&newChar.playerName, playerName, strlen(playerName));
			newChar.playerName[strlen(playerName)] = '\0';
		}
		else{log << ERRORX << "Increse NewPlayerBaseInfo::playerName size" << endl; return false;}
	}
	else{log << ERRORX << "Increse NewPlayerBaseInfo::modelName size" << endl; return false;}

	// Setting the rest of the values in the message
	newChar.ePackageType = NEW_PLAYER;
	newChar.startXPos = charToSendInfoFrom.getPositionX();
	newChar.startYPos = charToSendInfoFrom.getPositionY();
	newChar.characterID = charToSendInfoFrom.GetImageID();
	// Loading the message into the outgoing msg manager
	_outMsg->LoadMessage(NEW_PLAYER, &newChar, frozen::newPlayerBaseInfoSize);
	_newMsgToSend = true;
	return true;
}

bool CNetManager::SendNewEnemyInfo( CEnemyObject& charToSendInfoFrom )
{
	NewPlayerBaseInfo newChar; // Creating a message object for new player info

	// Copying the model and player name into our message object, returning false if it fails.
	const char* model = charToSendInfoFrom.GetSurfaceName().c_str();
	if(sizeof(newChar.modelName) >= strlen(model)+1)
	{
		memcpy(&newChar.modelName, model, strlen(model));
		newChar.modelName[strlen(model)] = '\0';

		const char* playerName = "enemy";
		if(sizeof(newChar.playerName) >= strlen(playerName)+1)
		{
			memcpy(&newChar.playerName, playerName, strlen(playerName));
			newChar.playerName[strlen(playerName)] = '\0';
		}
		else{log << ERRORX << "Increse NewPlayerBaseInfo::playerName size" << endl; return false;}
	}
	else{log << ERRORX << "Increse NewPlayerBaseInfo::modelName size" << endl; return false;}

	// Setting the rest of the values in the message
	newChar.ePackageType = NEW_PLAYER;
	newChar.startXPos = charToSendInfoFrom.getPositionX();
	newChar.startYPos = charToSendInfoFrom.getPositionY();
	newChar.characterID = charToSendInfoFrom.GetImageID();
	// Loading the message into the outgoing msg manager
	_outMsg->LoadMessage(NEW_PLAYER, &newChar, frozen::newPlayerBaseInfoSize);
	_newMsgToSend = true;
	return true;
}

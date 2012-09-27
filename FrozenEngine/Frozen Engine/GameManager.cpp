#include "GameManager.h"

// P U B L I C  F U N C T I O N S

CGameMan::CGameMan(GameSave* gameToLoad) 
	: log("GameManager", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)								
{
	background = NULL;
	saveGame = NULL;
	player_local = NULL;
	_isSinglePlayer = true;
	_netManager = NULL;
	
	/* If gameToLoad != NULL, we create a player character from the gameSave */
	/*if(gameToLoad)
	{
	int players = gameToLoad->Characters.size();
	std::string modelName = gameToLoad->Characters.at(0)->modelName;
	std::string playerName = gameToLoad->Characters.at(0)->playerName;
	float xPos = gameToLoad->Characters.at(0)->xPos;
	float yPos = gameToLoad->Characters.at(0)->yPos;
	player_local = new CTestPlayer(modelName, playerName, xPos, yPos);
	}
	else*/
	{
		
	}
}

CGameMan::CGameMan(CNetManager* netManager) 
	: log("GameManager", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)								
{
	background = NULL;
	saveGame = NULL;
	player_local = NULL;
	_isSinglePlayer = false;
	_netManager = netManager;
}

CGameMan::~CGameMan()
{
}

// D E R I V E D  F U N C T I O N S

bool CGameMan::Start()
{
	CSound::Inst()->StopMusic();
	CSound::Inst()->PlayMusic("game");

	////CRenderEngine::Inst()->ClearImageList();

	//aGravity.setY(1000); aGravity.setX(0);

	background = new CStaticBackground("background");
	
	
	/*if(!player_local)
	{
		if(_netManager)
		{
			if(_netManager->IsHost())
			{
				player_local = new CTestPlayer("naruto", "Naruto", 300, 300);
			}
			else
			{
				player_local = new CTestPlayer("sasuke", "Sasuke", 300, 300);
			}
		}
	}*/
	player_local = new CTestPlayer("naruto", "Kristian", 200, 200);
	playerCharacters.push_back(player_local);
	
	//if(!_netManager)
	//{
	//	//enemyObject = new CEnemyObject("tobi", 100, 500);
	//	//CRenderEngine::Inst()->AddRenderImg(enemyObject);
	//	//enemiesVector.push_back(enemyObject);
	//}

	//if(_netManager)
	//{
	//	if(_netManager->IsHost())
	//	{
	//		//enemyObject = new CEnemyObject("tobi", 100, 500);
	//		//CRenderEngine::Inst()->AddRenderImg(enemyObject);
	//		//enemiesVector.push_back(enemyObject);
	//	}
	//	//_netManager->SetLocalPlayersVectorRef(&playerCharacters);
	//	//_netManager->SetLocalEnemiesVectorRef(&enemiesVector);
	//	static GCPtr<Task> NetUpdates(_netManager);
	//	CKernel::Inst()->AddTaskForNextUpdate(NetUpdates);
	//}
	
	//if(player_local)
	//{
	//	//player_local->SetImageID(1);
	//}
	////if(enemyObject)
	//{
	//	//enemyObject->SetImageID(2);
	//}
	
	return true;
}

void CGameMan::Stop()
{
	CSound::Inst()->StopMusic();

	if(_netManager)
	{
		_netManager->DisconnectNetManager();
	}
	
	if(_isSinglePlayer)
	{
		saveGame = new CXmlSaveGames();
		saveGame->SaveGame(1, *player_local);
		if(saveGame)
		{
			delete saveGame;
			saveGame = 0;
		}	
	}

	playerCharacters.clear();
	enemiesVector.clear();
	//if(enemyObject)
	//{
	//	delete enemyObject;
	//	enemyObject = 0;
	//}
	if (player_local)
	{
		delete player_local;
		player_local = 0;
	}
	if(background)
	{
		delete background;
		background = 0;
	}
}

void CGameMan::Update()
{
	float deltaTicks = CTimer::Inst()->getDeltaTime();

	_UserInput(); //G et user input
	
	if(player_local)
	{
		player_local->UpdateSurface();
	}
	
	_WallCollisionCheck();

	_printFPS();
}

// P R I V A T E  F U N C T I O N S

void CGameMan::_WallCollisionCheck()
{
	//for(auto i = playerCharacters.begin(); i != playerCharacters.end(); i++)
	//{
	//	if((*i)->GetYPosition() > 600.0F - (*i)->getImageHeight())
	//	{
	//		(*i)->setPositionY(600.0F - (*i)->getImageHeight());
	//		(*i)->setVelocityY(0);
	//	}

	//	if((*i)->getPositionY() < 0)
	//	{
	//		(*i)->setPositionY(0);
	//		(*i)->setVelocityY(0);
	//	}

	//	if((*i)->getPositionX() > 800.0F - (*i)->getImageWidth())
	//	{
	//		(*i)->setPositionX(800.0F - (*i)->getImageWidth());
	//		(*i)->setVelocityX(0);
	//	}

	//	if((*i)->getPositionX() < 0)
	//	{
	//		(*i)->setPositionX(0);
	//		(*i)->setVelocityX(0);
	//	}
	//}
}

void CGameMan::_UserInput()
{
	//Jump - Space
	if (CInput::Inst()->Jump())
	{
		player_local->Jump();
	}
	//Mute - M
	if (CInput::Inst()->MuteSound())
	{
		//mute
	}

	//Horizontal Movement
	if (CInput::Inst()->MoveLeft() || CInput::Inst()->MoveRight())
	{
		//if left and right
		if (CInput::Inst()->MoveLeft() && CInput::Inst()->MoveRight())
		{
			player_local->SetXVelocity(0);
		}
		//If only left
		if (CInput::Inst()->MoveLeft() && !CInput::Inst()->MoveRight())
		{
			player_local->MoveLeft(false);
		}
		//If only right
		if (!CInput::Inst()->MoveLeft() && CInput::Inst()->MoveRight())
		{
			player_local->MoveRight(false);
		}
	}
	else
	{
		//No movement
		player_local->SetXVelocity(0);
	}

	//Exit game - Esc
	if (CInput::Inst()->ExitGame())
	{
		GCPtr<Task> GameMenu( new CMenu());
		CKernel::Inst()->AddTaskForNextUpdate(GameMenu);
		FlagForKill();
	}
}

void CGameMan::_printFPS()
{
	int FPS = CTimer::Inst()->getFPSPrint();
	int wantedFPS = CTimer::Inst()->getWantedFPS();

	std::ostringstream fpsStr;
	fpsStr << "Frozen Engine 0.801 FPS: ";

	if (FPS <=  wantedFPS )
	{
		if(FPS+1 == wantedFPS) 
		{
			fpsStr<< wantedFPS;//Remove inaccurate flickering
		}
		else
		{
			fpsStr << FPS;
		}
	} else{ fpsStr << "--"; }
	SDL_WM_SetCaption(fpsStr.str().c_str(), 0);
}
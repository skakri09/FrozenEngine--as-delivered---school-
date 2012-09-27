#include "Menu.h"
#include "BoostButton.h"/**/
#include <boost/bind.hpp>
CNetManager* CMenu::netManager = NULL;
bool CMenu::isFirstCtorCall = true;

// P U B L I C  F U N C T I O N S

CMenu::CMenu( )
	: log("Menu", CXmlSettings::Inst()->GetLoggerSettings()->logLevel) 
{
	btnStartHost = NULL;
	btnStartClient = NULL;
	btnStartSingleplayer = NULL;
	btnExit = NULL;
	menuBG = NULL;
	isPlayingMusic = false;
	
	if(isFirstCtorCall)
	{
		isFirstCtorCall = false;
		//CRenderEngine::Inst()->StartMovie("EngineIntro720p");
	}
}

CMenu::~CMenu()
{
}

// D E R I V E D  F U N C T I O N S

bool CMenu::Start()
{
	//GCPtr<int>::collect();

	CSound::Inst()->setGlobalGain(0.5F);
	currentDigit = 0;
	ipStr = "";
	ipInfo = "FROZEN ENGINE - If client, Type in IP: ";
	savedGames = new CXmlSaveGames();

	//CRenderEngine::Inst()->ClearImageList();

	menuBG = new CRenderableNoAnimation("menu_bg");

	/*boostStartGame = boost::bind(&CMenu::RunGame, this, _1);
	btnStartSingleplayer = new CButton(boostStartGame,"single_player", 280, 155);
	btnStartClient = new CButton(boostStartGame, "start_client", 280, 305);
	btnStartHost = new CButton(boostStartGame, "start_server", 280, 230);
	btnExit = new CButton(boost::bind(&CMenu::QuitGame, this),"exit", 280, 380);*/

	btnStartSingleplayer = new CButton("single_player", 280, 155);
	btnStartClient = new CButton("start_client", 280, 305);
	btnStartHost = new CButton("start_server", 280, 230);
	btnExit = new CButton("exit", 280, 380);
	
	return true;
}

void CMenu::Stop()
{
	CSound::Inst()->StopMusic();

	/*if(CRenderEngine::Inst()->PlayingMovie())
	{
		CRenderEngine::Inst()->StopMovie();
	}*/
	if(savedGames)
	{
		delete savedGames;
		savedGames = 0;
	}
	if (btnStartHost)
	{
		log << INFO << "Deleting btnStartHost" << endl;
		delete btnStartHost;
		btnStartHost = 0;
	}
	if (btnStartClient)
	{
		log << INFO << "Deleting btnStartClient" << endl;
		delete btnStartClient;
		btnStartClient = 0;
	}
	if (menuBG)
	{
		log << INFO << "Deleting menuBG" << endl;
		delete menuBG;
		menuBG = 0;
	}
	if (btnExit)
	{
		log << INFO << "Deleting btnExit" << endl;
		delete btnExit;
		btnExit = 0;
	}
}

void CMenu::Update()
{
	/*if(!isPlayingMusic && !CRenderEngine::Inst()->PlayingMovie())
	{
		isPlayingMusic = true;
		CSound::Inst()->PlayMusic("FrozenMenu");
	}*/
	
	if(btnStartSingleplayer->LeftReleased())
	{
		RunGame(false);
	}

	if(btnStartHost->LeftReleased())
	{
		//InitNetwork(false);
		RunGame(InitNetwork(false));
	}
	if(btnStartClient->LeftReleased())
	{
		//InitNetwork(true);
		RunGame(InitNetwork(true));
	}
	if(btnExit->LeftReleased())
	{
		QuitGame();
	}
	
	if( btnExit->MouseIsOver()			||
		btnStartClient->MouseIsOver()	||
		btnStartHost->MouseIsOver()		||
		btnStartSingleplayer->MouseIsOver())
	{
		CSound::Inst()->PlaySound("button");
	}

	log << INFO << btnStartHost->GetBtnState() << endl;


	writeIP();
	
	CheckForQuit();
}

// P R I V A T E  F U N C T I O N S

void CMenu::RunGame(bool multiPlayer)
{
	if(multiPlayer)
	{
		GCPtr<Task> GameUpdate( new CGameMan(netManager));
		CKernel::Inst()->AddTaskForNextUpdate(GameUpdate);

		//CRenderEngine::Inst()->ClearImageList();
	}
	else
	{
		GCPtr<Task> GameUpdate(new CGameMan(savedGames->GetSavedGame(1)));
		CKernel::Inst()->AddTaskForNextUpdate(GameUpdate);

		//CRenderEngine::Inst()->ClearImageList();
	}
	FlagForKill();
}

void CMenu::CheckForQuit()
{
	if (CInput::Inst()->ExitGame())
	{
		/*if(!CRenderEngine::Inst()->PlayingMovie())
		{
			
		}*/
		CKernel::Inst()->RemoveAllTasks();
	}
}

bool CMenu::InitNetwork(bool asClient)
{
	int _port = CXmlSettings::Inst()->GetMultiiplayerSettings()->port;
	//bool _isClient = CXmlSettings::Inst()->GetMultiiplayerSettings()->playerIsClient;
	bool _isClient = asClient;
	std::string _oponentIP = CXmlSettings::Inst()->GetMultiiplayerSettings()->opponentIP;
	_port = 2000;
	if(!netManager)
	{
		if(_isClient)
		{
			if (currentDigit > 0)
			{
				netManager = new CNetManager(ipStr, _port);
			}
			else
			{
				netManager = new CNetManager(_oponentIP, _port);
			}
		}
		else
		{	
			netManager = new CNetManager(_port);
		}
	}
	else
	{
		netManager->ChangeToClientOrHost(!_isClient);
	}

	if(netManager)
	{
		return true;
	}
	else{ return false;}
}

void CMenu::writeIP()
{
	string inChar = "";
	string tempStr = "";
	bool backspace = false;

	if(CInput::Inst()->KeyDown(SDLK_0))
		inChar = "0";
	if(CInput::Inst()->KeyDown(SDLK_1))
		inChar = "1";
	if(CInput::Inst()->KeyDown(SDLK_2))
		inChar = "2";
	if(CInput::Inst()->KeyDown(SDLK_3))
		inChar = "3";
	if(CInput::Inst()->KeyDown(SDLK_4))
		inChar = "4";
	if(CInput::Inst()->KeyDown(SDLK_5))
		inChar = "5";
	if(CInput::Inst()->KeyDown(SDLK_6))
		inChar = "6";
	if(CInput::Inst()->KeyDown(SDLK_7))
		inChar = "7";
	if(CInput::Inst()->KeyDown(SDLK_8))
		inChar = "8";
	if(CInput::Inst()->KeyDown(SDLK_9))
		inChar = "9";
	if(CInput::Inst()->KeyDown(SDLK_PERIOD))
		inChar = ".";
	if(CInput::Inst()->KeyDown(SDLK_BACKSPACE))
		backspace = true;
	
	if (backspace)
	{
		ipStr = (ipStr.substr(0, ipStr.size()-1));
		currentDigit--;
	}
	if (inChar != "")
	{
		ipStr.append(inChar);
		currentDigit++;
	}
	tempStr = ipInfo + ipStr;

	SDL_WM_SetCaption(tempStr.c_str(), 0);
}

void CMenu::QuitGame()
{
	CKernel::Inst()->RemoveAllTasks();
}


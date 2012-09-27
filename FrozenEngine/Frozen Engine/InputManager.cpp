#include "InputManager.h"

// P U B L I C   F U N C T I O N S

GCPtr<CInput> CInput::Inst()
{
	static GCPtr<CInput> instance = new CInput();
	return instance;
}

CInput::CInput() : log("InputManager", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	keys = 0;
	oldKeys = 0;
	keyCount = 0;

	mouseX = 0;
	mouseY = 0;
	buttons = 0;
	oldButtons = 0;

	//Control settings
	JumpKey = CXmlSettings::Inst()->GetControlSettings()->jump;
	MoveRightKey = CXmlSettings::Inst()->GetControlSettings()->moveRight;
	MoveLeftKey = CXmlSettings::Inst()->GetControlSettings()->moveLeft;
	menuKey = CXmlSettings::Inst()->GetControlSettings()->menu;
	muteKey = CXmlSettings::Inst()->GetControlSettings()->mute;
}

mouseCoords CInput::getMousePos()
{
	mouseCoords mCoords;

	mCoords.x = mouseX;
	mCoords.y = mouseY;

	return mCoords;
}

bool CInput::KeyDown( int keyID )
{
	return ( ( _CurrentKeyIsDown(keyID)) && (!_OldKeyIsDown(keyID)) );
}

bool CInput::KeyStillDown( int keyID )
{
	return ( ( _CurrentKeyIsDown(keyID)) && ( _OldKeyIsDown(keyID)) );
}

bool CInput::KeyUp( int keyID )
{
	return ( (!_CurrentKeyIsDown(keyID)) && ( _OldKeyIsDown(keyID)) );
}

bool CInput::KeyStillUp( int keyID )
{
	return ( (!_CurrentKeyIsDown(keyID)) && (!_OldKeyIsDown(keyID)) );
}

bool CInput::MouseDown( int buttonID )
{
	return ( ( _CurrentButtonIsDown(buttonID)) && (!_OldButtonIsDown(buttonID)) ); 
}

bool CInput::MouseStillDown( int buttonID )
{
	return ( ( _CurrentButtonIsDown(buttonID)) && ( _OldButtonIsDown(buttonID)) );
}

bool CInput::MouseUp( int buttonID )
{
	return ( (!_CurrentButtonIsDown(buttonID)) && ( _OldButtonIsDown(buttonID)) );
}

bool CInput::MouseStillUp( int buttonID )
{
	return ( (!_CurrentButtonIsDown(buttonID)) && (!_OldButtonIsDown(buttonID)) );
}

bool CInput::Jump()
{
	if (KeyDown(JumpKey))
	{
		return true;
	}
	return false;
}

bool CInput::MoveLeft()
{
	if (KeyDown(MoveLeftKey) || KeyStillDown(MoveLeftKey))
	{
		return true;
	}
	return false;
}

bool CInput::MoveRight()
{
	if (KeyDown(MoveRightKey) || KeyStillDown(MoveRightKey))
	{
		return true;
	}
	return false;
}

bool CInput::MuteSound()
{
	if (KeyDown(muteKey))
	{
		return true;
	}
	return false;
}

bool CInput::ExitGame()
{
	if (KeyDown(menuKey))
	{
		return true;
	}
	return false;
}

// D E R I V E D  F U N C T I O N S

bool CInput::Start()
{
	//SDL_VIDEO must be inited in the thread that is using SDL Events
	if (!SDL_WasInit(SDL_INIT_VIDEO))
	{
		if (0 > SDL_Init(SDL_INIT_VIDEO))
		{
			log << CRITICAL << "SDL_INIT_VIDEO Could not be initialized - User input disabled." << std::endl;
			return false;
		}
	}

	SDL_EnableKeyRepeat(0, 50);

	keys = SDL_GetKeyState(&keyCount);

	oldKeys = new Uint8[keyCount];

	//Copy keys to oldkeys
	memcpy(oldKeys, keys, sizeof(Uint8) * keyCount);

	//Call pump event twice to flush any previous inputs
	SDL_PumpEvents();
	SDL_PumpEvents();

	return true;
}

void CInput::Stop()
{
}

void CInput::Update()
{
	//Copy keys to oldkeys
	memcpy(oldKeys, keys, sizeof(Uint8) * keyCount);

	SDL_PumpEvents();

	//Update mouse input
	oldButtons = buttons;
	buttons = SDL_GetMouseState(&mouseX, &mouseY);	
}


// P R I V A T E  F U N C T I O N S

bool CInput::_CurrentKeyIsDown( int keyID )
{
	return (keys[keyID] != 0);
}

bool CInput::_OldKeyIsDown( int keyID )
{
	return (oldKeys[keyID] != 0);
}

bool CInput::_CurrentButtonIsDown( int buttonID )
{
	return ((buttons&SDL_BUTTON(buttonID)) != 0);
}

bool CInput::_OldButtonIsDown( int buttonID )
{
	return ((oldButtons&SDL_BUTTON(buttonID)) != 0);
}

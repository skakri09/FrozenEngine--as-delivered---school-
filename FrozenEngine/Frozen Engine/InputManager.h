/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   20:17
	filename: 	InputManager.h
	file path:	FrozenEngine\Frozen Engine
	file base:	InputManager
	file ext:	h
	author:		Peder Thorup, Kristian Skarseth
	
	purpose:	Keeps a copy of SDL's internal array to keep track of 
				input from user.
				Also contains helper functions that can be used to 
				navigate player.
				Using Singleton DP
*********************************************************************/

#ifndef InputManager_h__
#define InputManager_h__

// I N C L U D E S

#include <SDL.h>

#include "Task.h"
#include "Logger.h"
#include "GCPtr.h"
#include "Kernel.h"
#include "xmlSettingsHandling.h"
#include "SoundManager.h"

using std::endl;

struct mouseCoords
{
	int x;
	int y;
};

// C L A S S   D E C L A R A T I O N

class CInput : public Task
{
public:
	static GCPtr<CInput> Inst();

	bool Start(); 
	void Stop();
	void Update();

	//key is pressed
	bool KeyDown(int keyID);
	//key is still down
	bool KeyStillDown(int keyID);
	//Key is released
	bool KeyUp(int keyID);
	//Key is already released
	bool KeyStillUp(int keyID);

	//Mouse button is pressed
	bool MouseDown(int buttonID);
	//Mouse button already down
	bool MouseStillDown(int buttonID);
	//Mouse button released
	bool MouseUp(int buttonID);
	//Mouse button already released
	bool MouseStillUp(int buttonID);

	//Returns mouse position
	mouseCoords getMousePos();

	//Helper functions to give better readability in gamemanager

	bool Jump();

	// Returns true if user moved character to the left
	bool MoveLeft(); 

	// Returns true if user moved character to the right
	bool MoveRight();

	// Returns true if character not moving horizontaly
	bool noHorizontalMovement();

	bool MuteSound();

	// Returns true if user Quits game by presseing ESC or X'ing out window
	bool ExitGame();

protected:
	// A pointer to SDL's internal key state array.
	Uint8* keys;
	// Our own copy of last update's array. 
	Uint8* oldKeys;
	// Number of keys in the arrays above.
	int keyCount;

	// Mouse pos x and y.
	int mouseX;
	int mouseY;
	// Mouse buttons pressed. 
	Uint8 buttons;
	// Mouse buttons pressed last update. 
	Uint8 oldButtons;

	int MoveLeftKey;
	int MoveRightKey;
	int JumpKey;
	int menuKey;
	int muteKey;

private:
	//static CInput* instance;

	CInput(void); // ctor hidden
	CInput(CInput const&);

	//Keyboard helper functions
	bool _CurrentKeyIsDown(int keyID);
	bool _OldKeyIsDown(int keyID);
	//Mouse helper functions
	bool _CurrentButtonIsDown(int buttonID);
	bool _OldButtonIsDown(int buttonID);

	Logger log;
};


#endif // InputManager_h__

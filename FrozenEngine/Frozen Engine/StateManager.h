/********************************************************************
*	Created:	2012/04/19  10:46
*	Filename: 	StateManager.h
*	Author:		Kristian Skarseth
*	
*	purpose:	Manage the different game states. Acts as a mid-station
				when switching the states, and makes sure to clean up
				any memory that's not being used anymore, after switching
				a state.
*********************************************************************/

#ifndef StateManager_H
#define StateManager_H

#include "GameManager.h"
#include "Menu.h"
#include "Kernel.h"
#include "Logger.h"
#include "xmlSettingsHandling.h"
#include "SoundManager.h"
#include "GameManager.h"
#include "RenderEngine.h"
#include "Timer.h"
#include "Task.h"
#include "GCPtr.h"
#include <set>
#include <SDL.h>
#include "RRenderEngine.h"

enum eGameState{MENU, GAME};

class CStateManager : public Task
{
public:
	CStateManager();
	~CStateManager();

	/* Inherited Task functions */
	bool Start();
	void Stop();
	void Update();

	/* Starts the engine core functionality */
	void Initialize();

	void RunGame(); /* Starts the game */

	void RunMenu(); /* Starts the menu */

private:
	Logger log;
	eGameState currGameState;
	eGameState nextGameState;

	void CleanupGame(); /* Cleans up after leaving a game */
	void CleanupMenu(); /* Cleans up after leaving menu   */

};

#endif // StateManager_H

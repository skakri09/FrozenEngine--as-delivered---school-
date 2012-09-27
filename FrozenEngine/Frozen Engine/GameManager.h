/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   21:59
	filename: 	GameManager.h
	file path:	FrozenEngine\Frozen Engine
	file base:	GameManager
	file ext:	h
	author:		Peder Thorup, Kristian Skarseth
	
	purpose:	Game manager. Nuff said
*********************************************************************/

#ifndef GameManager_h__
#define GameManager_h__

// I N C L U D E S

#include <SDL.h>
#include <vector>

#include "Task.h"
#include "Kernel.h"
#include "Vector2D.h"
#include "GCPtr.h"
#include "Timer.h"
#include "RenderEngine.h"
#include "xmlSettingsHandling.h"
#include "xmlSaveGames.h"
#include "SoundManager.h"
#include "NetManager.h"
#include "Logger.h"
#include "Menu.h"
#include "StaticBackground.h"
#include "TestPlayer.h"

// C L A S S  D E C L A R A T I O N
class CGameMan : public Task
{
public:
	/* Ctor used for singleplayer games. an optional GameSave* can be 	*/
	/* passed as argument if we want to load a saved game.				*/
	CGameMan(GameSave* gameToLoad = NULL);	
	
	/* Ctor used for multiplayer games. We get a reference to the		*/
	/* netManager trough param, and stores a local copy of it.			*/
	CGameMan(CNetManager* netManager);

	~CGameMan();

	/* Inherited Task functions */
	bool Start();
	void Stop();
	void Update();

private:
	Logger log;					/* Our logger object										*/

	void _UserInput();			/* Handles input for the game using calls to inputManager	*/
	
	void _WallCollisionCheck();	/* Simple "placeholder" collision check to not allow		*/
								/* characters outside the screen.							*/		
	
	void _printFPS();			/* Print FPS to application title */

	bool _isSinglePlayer;		/* True when we we're singleplayer, false when multiplayer	*/
	
	CXmlSaveGames* saveGame;		/* Our local saveGame object				*/
	CNetManager* _netManager;		/* Our local pointer to the netManager		*/

	//CImageObject* background;		/* ImageObject, our background		*/
	//CCharacterObject* player_local;	/* CharacterObject, our player		*/
	//CCharacterObject* charObject;	/* An empty character object		*/
	//CEnemyObject* enemyObject;		/* An enemy object with a simpleAI	*/
	
	CStaticBackground* background;		/* ImageObject, our background		*/
	CTestPlayer* player_local;	/* CharacterObject, our player		*/
	
	//CEnemyObject* enemyObject;		/* An enemy object with a simpleAI	*/
	
	/*	Vector of our player characters	*/
	std::vector<CTestPlayer*> playerCharacters; 
	std::vector<CEnemyObject*> enemiesVector;
};

#endif // GameManager_h__
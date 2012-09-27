/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   20:26
	filename: 	Menu.h
	file path:	FrozenEngine\Frozen Engine
	file base:	Menu
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	Game menu where the user can type in ip, select to be host
				or client
*********************************************************************/

#ifndef Menu_h__
#define Menu_h__

// I N C L U D E S

#include <string>

#include "Button.h"
#include "Logger.h"
#include "ImageObject.h"
#include "xmlSaveGames.h"
#include "GameManager.h"
#include "SoundManager.h"
#include "GUI_Interface.h"
#include "RRenderEngine.h"
#include "TestPlayer.h"
#include "RenderableNoAnimation.h"

// C L A S S  D E C L A R A T I O N

class CXmlSaveGames;
class CMenu : public Task
{
public:
	CMenu();
	~CMenu();

	/* Derived Task functions */
	bool Start(); 
	void Stop();
	void Update();

private:
	Logger log; /* Logger object */
	
	/* a static pointer to the netManager */
	static CNetManager* netManager;
	
	/* a static bool to determine if it's */
	/* the first time we run the Ctror.	  */
	static bool isFirstCtorCall;

	/* Initializes the network	*/
	bool InitNetwork(bool asClient);
	
	/* Checks for events that tells us to quit. Does so if */
	void CheckForQuit();
	
	/* Starts the game, also flags menu task for kill */
	void RunGame(bool singlePlayer);
	
	//Lets the user type in an IP to connect to.
	void writeIP();

	CButton* btnStartSingleplayer;	/* Button to start singlePlayer game	*/
	CButton* btnStartHost;			/* Button to start a game as a host		*/
	CButton* btnStartClient;		/* Button to start a game as a client	*/
	CButton* btnExit;				/* Button to exit the engine			*/
	CRenderableNoAnimation* menuBG;			/* Background image						*/

	/* SaveGames object we create and destroy each time we	*/
	/* restart the menu to get the updated game saves		*/
	//Application text ip input
	string ipStr;
	string ipInfo;
	int currentDigit;

	bool isPlayingMusic;

	CXmlSaveGames* savedGames;

	void QuitGame();
};



#endif // Menu_h__
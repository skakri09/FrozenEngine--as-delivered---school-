/********************************************************************
	created:	2012/04/05
	created:	5:4:2012   11:03
	filename: 	xmlSaveGames.h
	file path:	FrozenEngine\Frozen Engine
	file base:	xmlSaveGames
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	Load and and create game saves from XML. A game save will
				include a levelID to determine what background and other 
				static environment to load, as well as an overview of all
				dynamically changing stuff that will require a specified 
				x and y coordinate + other relevant information. 

				The class supports "limitless" number of saves, and you 
				can load a selected save with an int ID (0 and up). 
*********************************************************************/

#ifndef xmlSaveGames_h__
#define xmlSaveGames_h__

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <vector>
#include <string>

#include "Logger.h"
//#include "GameManager.h"
#include "xmlSettingsHandling.h"
#include "TestPlayer.h"

using boost::property_tree::ptree;



/* Struct for information to save for a player */
struct Player 
{
	std::string playerName;
	std::string modelName;
	float xPos;
	float yPos;
};

/*
// Struct for information to save for an enemy 
struct Enemy
{
	std::string modelName; // Name of the model the player should use
	float xPos;
	float yPos;
};
*/

// Each save consist of one GameSave struct
struct GameSave 
{
	int saveNumber; // The save number this save is
	int levelID; // The level to load (environment etc)
	std::vector<Player*> Characters; // Vector with info of all the players to save/load
	//std::vector<Enemy*> Enemies;	// Vecor with info of all the enemies to save/load
};

using std::endl;
static const char* saveGamesXmlpath = "..//XML//GameSaves.xml";
typedef std::vector<Player*>::iterator playersIter;


class CXmlSaveGames
{
public:
	CXmlSaveGames();
	~CXmlSaveGames();

	/* Creates a saveGame and writes it to the XML. Currently only takes */
	/* one CCharacterObject and saves it, but can easily be changed to   */
	/* receive a container of players/enemies/surfaces to store info on. */
	void SaveGame(int lvlID, CTestPlayer& player);
	
	/* Returns a pointer to the SaveGame struct on the saveNumber position*/
	/* in our _gameSavesVec, if it exists. Otherwise it returns NULL	  */
	GameSave* GetSavedGame(int saveNumber);

private:
	Logger log;	/* Our logger object */
	
	/* Loads all (if any) saved games from xml into _allSavesPtree and */
	/* adds each save to a GameSave struct and put it in _gameSavesVec */
	void _LoadXMLToStructsAndPtree();

	/* Writes the _allSavesPtree to our saveGames XML */
	void _WriteSavesToXml();
	
	/* Frees dynamically allocated memory in _gameSavesVec, and clears it.*/
	void _FreeAndClearGameSavesVec();

	/* Ptree holding all the GameSaves */
	ptree _allSavesPtree;

	Player* _player;		/* Player object	*/
	GameSave* _gameSave;	/* gameSave object	*/

	/* Vector of all gamesaves	*/
	std::vector<GameSave*> _gameSavesVec;
};

#endif // xmlSaveGames_h__

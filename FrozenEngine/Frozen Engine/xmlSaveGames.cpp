#include "xmlSaveGames.h"

/* P U B L I C    F U N C T I O N S */
/*----------------------------------*/

CXmlSaveGames::CXmlSaveGames() : log("xmlSaveGames", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	_LoadXMLToStructsAndPtree();
}

CXmlSaveGames::~CXmlSaveGames()
{
	_FreeAndClearGameSavesVec();
}

void CXmlSaveGames::SaveGame(int lvlID, CTestPlayer& player)
{	
	// Creating a new gameSave
	_gameSave = new GameSave;	
	// Giving the save an ID
	if(_gameSavesVec.empty())
	{
		_gameSave->saveNumber = 1;
	}
	else
	{
		_gameSave->saveNumber = _gameSavesVec.size()+1;
	}
	log << INFO << "Starting to save game: " << _gameSave->saveNumber <<  endl;

	_gameSave->levelID = lvlID; // Setting levelID of the save

	// Creating a new player object for each player, adding values and placing in gameSave
	// for(auto i = player.begin(); i != player.end(); i++)
	{
		_player = new Player;
		//_player->modelName = player.GetSurfaceName();//(*i)
		//_player->playerName = player.GetPlayerName();//(*i)->GetPlayerName();
		_player->xPos = player.GetXPosition();//(*i)->getPositionX();
		_player->yPos = player.GetYPosition();//(*i)->getPositionY();
		_gameSave->Characters.push_back(_player);
	}

	// Adding gameSave to vector of all gameSaves
	_gameSavesVec.push_back(_gameSave);

	_WriteSavesToXml();
	log << INFO << "Finished saving game" << endl;
}


GameSave* CXmlSaveGames::GetSavedGame( int saveNumber )
{
	if(!_gameSavesVec.empty())
	{
		return _gameSavesVec.at(saveNumber-1);
	}
	else return NULL;
}



/* P R I V A T E     F U N C T I O N S */
/*-------------------------------------*/

void CXmlSaveGames::_LoadXMLToStructsAndPtree()
{
	log << INFO << "Loading saved games..." << endl;
	read_xml(saveGamesXmlpath, _allSavesPtree);
	if(!_allSavesPtree.empty())
	{
		ptree saves = _allSavesPtree.get_child("GameSaves");
		for(ptree::iterator i = saves.begin(); i != saves.end(); i++)
		{
			ptree saveInfo = i->second;
			_gameSave = new GameSave;
			_gameSave->saveNumber = saveInfo.get<int>("SaveID", 0);
			_gameSave->levelID = saveInfo.get<int>("LevelID", 0);

			ptree chars = saveInfo.get_child("Characters");
			for(ptree::iterator i = chars.begin(); i !=chars.end(); i++)
			{
				ptree character = i->second;
				_player = new Player;
				_player->modelName = character.get<std::string>("Model", "no_surface");
				_player->playerName = character.get<std::string>("Name", "noname");
				_player->xPos = character.get<float>("xPos", 0);
				_player->yPos = character.get<float>("yPos", 0);
				_gameSave->Characters.push_back(_player);
			}

			_gameSavesVec.push_back(_gameSave);
		}
		log << INFO << "finished loading " << _gameSavesVec.size() << " saved games."  << endl;
	}
	else
	{
		log << CRITICAL << "There are no saved games" << endl;
	}
}


void CXmlSaveGames::_WriteSavesToXml()
{
	for(auto iter = _gameSavesVec.begin(); iter != _gameSavesVec.end(); iter++)
	{
		ptree thisSave;
		thisSave.add("SaveID", (*iter)->saveNumber);
		thisSave.add("LevelID", (*iter)->levelID);

		for(playersIter i = (*iter)->Characters.begin();
			i != (*iter)->Characters.end(); i++)
		{
			ptree playerInfo;
			playerInfo.add("Model", (*i)->modelName);
			playerInfo.add("Name", (*i)->playerName);
			playerInfo.add("xPos", (*i)->xPos);
			playerInfo.add("yPos", (*i)->yPos);
			thisSave.add_child("Characters.Character", playerInfo);
		}

		/* Clearing the saves Ptree before writing to XML! this means we will only every save	*/
		/* one save at a time. Remove to keep old saves when creating a new save.				*/
		_allSavesPtree.clear();

		_allSavesPtree.add_child("GameSaves.GameSave", thisSave);
	}


	log << INFO << "Writing saveGame to xml" << endl;
	write_xml(saveGamesXmlpath, _allSavesPtree);
	log << INFO << "saveGame done writing to xml" << endl;

}


void CXmlSaveGames::_FreeAndClearGameSavesVec()
{
	for(auto iOuter = _gameSavesVec.begin(); iOuter!= _gameSavesVec.end(); iOuter++)
	{	
		for(auto iInner = (*iOuter)->Characters.begin(); iInner != (*iOuter)->Characters.end(); iInner++)
		{
			delete *iInner;
		}
		delete *iOuter;
		*iOuter = 0;
	}
	_gameSavesVec.clear();
}

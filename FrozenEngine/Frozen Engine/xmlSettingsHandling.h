/********************************************************************
	created:	2012/03/18
	created:	18:3:2012   17:11
	filename: 	xmlSettingsHandling.h
	file path:	FrozenEngine\GameEngine
	file base:	xmlSettingsHandling
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	Import/export settings from/to specified xml
				All settings are stored in structs, and settings 
				can be changed, applied or reset to default at runtime. 
	
	Note:		A default setting must be declared in each settings pt.get
				function call. Changed settings will only be written to XML
				after calling ApplySettingChanges().

*********************************************************************/
#ifndef XML_SETTINGS_HANDLING_H
#define XML_SETTINGS_HANDLING_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <vector>
#include <string>
#include <SDL_keysym.h>

#include "GCPtr.h"
#include "Logger.h"

struct VideoSettings
{
	unsigned short width;
	unsigned short height;
	int wantedFps;
	bool fullscreen;
	bool noFrame;
};
struct AudioSettings
{
	bool mute;
	float masterVolume;
	float effectsVolume;
	float musicVolume;
	float ambienceVolume;
};
struct ControlSettings
{
	SDLKey moveLeft;
	SDLKey moveRight;
	SDLKey jump;
	SDLKey duck;
	SDLKey attack;
	SDLKey pause;
	SDLKey menu;
	SDLKey mute;
};
struct MultiplayerSettings
{
	bool playerIsClient;
	std::string opponentIP;
	int port;
};
struct LoggerSettings
{
	bool logToConsole;
	Level logLevel;
};

static const char* SettingsXMLpath = "..//XML//Settings.xml";
using boost::property_tree::ptree;
using std::endl;

class CXmlSettings
{
public:
	//static CXmlSettings* Inst();
	static GCPtr<CXmlSettings> Inst();

	// Templated function to replace the value of a specified node
	template <typename T> void ChangeSetting(const std::string& nodePath, T value);

	// Populates the ptree (pt) from XML specified in global char* SettingsXMLpath
	void ReadXMLtoPtree();

	// Writes the ptree (pt) to XML specified in global char* SettingsXMLpath
	void WritePtreeToXML();

	// Puts temporary changes into the main ptree 
	// and updates all setting structs
	void ApplySettingChanges();

	// Resets the temporary ptree to the main ptree
	void DiscardChanges();

	// Loads the default settings by clearing the main ptree and refreshing 
	// all setting structs. Default values specified in the load-functions 
	// will then be both loaded in the main ptree and the setting structs
	void LoadDefaultSettings();

	// Returns pointer to a settings struct
	VideoSettings* GetVideoSettings();
	AudioSettings* GetAudioSettings();
	ControlSettings* GetControlSettings();
	MultiplayerSettings* GetMultiiplayerSettings();
	LoggerSettings* GetLoggerSettings();

private:
	CXmlSettings();
	~CXmlSettings();
	bool editedValues;
	Logger log;

	ptree pt;    // Main ptree with settings that are applied
	ptree temPt; // Temporary ptree for none-applied setting changes

	// Puts default values in all 
	void PutStructValsInPtree();
	
	// Refreshes all the setting structs at once
	void RefreshAllSettingStructs();
	// Refreshes the specific settings struct
	void RefreshVideoSettingsStruct();
	void RefreshAudioSettingsStruct();
	void RefreshControlSettingsStruct();
	void RefreshMultiplayerSettingsStruct();
	void RefreshloggerSettingsStruct();

	VideoSettings		videoSettings;
	AudioSettings		audioSettings;
	ControlSettings		controls;
	MultiplayerSettings multiPlayerSettings;
	LoggerSettings		loggerSettings;
};

#endif // XML_SETTINGS_HANDLING_H

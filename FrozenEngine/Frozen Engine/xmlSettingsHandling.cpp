#include "xmlSettingsHandling.h"

/* P U B L I C    F U N C T I O N S */
/*----------------------------------*/
//
//CXmlSettings* CXmlSettings::instance = 0;
//
//CXmlSettings* CXmlSettings::Inst()
//{
//	if(!instance)
//	{
//		instance = new CXmlSettings();
//	}
//	return instance;//todo delete
//}

GCPtr<CXmlSettings> CXmlSettings::Inst()
{
	static GCPtr<CXmlSettings> instance = new CXmlSettings();
	return instance;
}

CXmlSettings::CXmlSettings() : log("xmlSetting", INFO)
{
	// Importing the settings XML into a ptree (pt)
	ReadXMLtoPtree();

	// Manually setting the loglevel to the value our XML holds, as we cant import it in ctor like in other classes.
	log.SetLogLevel(static_cast<Level>(pt.get<int>("settings.logging.loglevel", 1)));

	// If the pt is empty after xml import (xml was empty), default settings are loaded and written to XML
	if(pt.empty())
	{
		LoadDefaultSettings();
		WritePtreeToXML();
	}
	else
	{
		RefreshAllSettingStructs();
	}
	temPt = pt;
	
	//ChangeSetting("settings.audio.mute", true);
	//ApplySettingChanges();

	//WritePtreeToXML();
	// Refreshing all the setting structs with newly imported xml in the ptree (pt)
}

CXmlSettings::~CXmlSettings()
{
	//Writing the ptree (pt) to the settings XML when exiting game.
	if(editedValues)
	{
		WritePtreeToXML();
	}
	
	/*delete instance;
	instance = 0;*/
}

template <typename T> void CXmlSettings::ChangeSetting(const std::string& nodePath, T value)
{
	// Example: "ChangeSetting("settings.audio.mute", true);"
	log << INFO << "Changing node: " << nodePath << " to: " << value << endl;
	try
	{
		temPt.put<T>(nodePath, value);	
		editedValues = true;
		log << INFO << "Successfully changed node" << endl;
	}
	catch(std::exception& e)
	{
		log << ERRORX <<" ERRROR while trying to change node: " << nodePath << endl;
	}
}

void CXmlSettings::ApplySettingChanges()
{
	pt = temPt;
	RefreshAllSettingStructs();
	WritePtreeToXML();
}

void CXmlSettings::DiscardChanges()
{
	temPt = pt;
}

void CXmlSettings::ReadXMLtoPtree()
{
	pt.clear();
	log << INFO << "Reading " << SettingsXMLpath << " into Ptree" << endl;
	read_xml(SettingsXMLpath, pt);
	log << INFO << "done reading xml" << endl;
}

void CXmlSettings::WritePtreeToXML()
{	
	//try
	{
		log << INFO << "Writing ptree to " << SettingsXMLpath << endl;
		write_xml(SettingsXMLpath, pt);
		log << INFO << "Done writing to XML" << endl;
	}
	/*
	catch(std::exception& e)
	{
		log << ERRORX << "ERROR while trying to write to XML: " << e << endl;
	}
	*/
}

VideoSettings* CXmlSettings::GetVideoSettings()
{
	return &videoSettings;
}

AudioSettings* CXmlSettings::GetAudioSettings()
{
	return &audioSettings;
}

ControlSettings* CXmlSettings::GetControlSettings()
{
	return &controls;
}

MultiplayerSettings* CXmlSettings::GetMultiiplayerSettings()
{
	return &multiPlayerSettings;
}

LoggerSettings* CXmlSettings::GetLoggerSettings()
{
	return &loggerSettings;
}

void CXmlSettings::LoadDefaultSettings()
{
	pt.clear(); // Clears the pt
	temPt.clear(); // Clears the temPt
	RefreshAllSettingStructs(); // Loads pt into all structs (since pt is empty, defaultValues are used)
	PutStructValsInPtree(); // Puts all struct values into pt
	temPt = pt; 
}


/* P R I V A T E     F U N C T I O N S */
/*-------------------------------------*/

void CXmlSettings::PutStructValsInPtree()
{
	log << INFO << "Starting struct-to-pt export of video settings" << endl;
	pt.put<int>("settings.video.resolution.width", videoSettings.width);
	pt.put<int>("settings.video.resolution.height", videoSettings.height);
	pt.put<int>("settins.video.wantedFps", videoSettings.wantedFps);
	pt.put<bool>("settings.video.fullscreen", videoSettings.fullscreen);
	pt.put<bool>("settings.video.noFrame", videoSettings.noFrame);
	log << INFO << "Video settings export done" << endl;

	/*------------------------------------------------------------------------------*/

	log << INFO << "Starting struct-to-pt export of audio settings" << endl;
	pt.put<bool>("settings.audio.mute", audioSettings.mute);
	pt.put<float>("settings.audio.volume.master", audioSettings.masterVolume);
	pt.put<float>("settings.audio.volume.effects", audioSettings.effectsVolume);
	pt.put<float>("settings.audio.volume.music", audioSettings.musicVolume);
	pt.put<float>("settings.audio.volume.ambience", audioSettings.ambienceVolume);
	log << INFO << "Audio settings export done" << endl;

	/*------------------------------------------------------------------------------*/

	log << INFO << "Starting struct-to-pt export of controls" << endl;
	pt.put<int>("settings.controls.movement.left", controls.moveLeft);
	pt.put<int>("settings.controls.movement.right", controls.moveRight);
	pt.put<int>("settings.controls.movement.jump", controls.jump);
	pt.put<int>("settings.controls.movement.duck", controls.duck);
	pt.put<int>("settings.controls.combat.attack", controls.attack);
	pt.put<int>("settings.controls.interface.pause", controls.pause);
	pt.put<int>("settings.controls.interface.menu", controls.menu);
	pt.put<int>("settings.controls.interface.mute", controls.menu);
	log << INFO << "Export of controls to XML done" << endl;

	/*------------------------------------------------------------------------------*/

	log << INFO << "Starting struct-to-pt export of multiplayer settings" << endl;
	pt.put<bool>("settings.multiplayer.playerisClient", multiPlayerSettings.playerIsClient);
	pt.put<std::string>("settings.multiplayer.opponentIP", multiPlayerSettings.opponentIP);
	log << INFO << "Export of multiplayer settings to XML done" << endl;

	/*------------------------------------------------------------------------------*/

	log << INFO << "Starting struct-to-pt export of logging settings" << endl;
	pt.put<bool>("settings.logging.logToConsole", loggerSettings.logToConsole);
	pt.put<int>("settings.logging.loglevel", loggerSettings.logLevel);
	log << INFO << "Export of logger settings to XML done" << endl;
}

void CXmlSettings::RefreshAllSettingStructs()
{
	RefreshVideoSettingsStruct();
	RefreshAudioSettingsStruct();
	RefreshControlSettingsStruct();
	RefreshMultiplayerSettingsStruct();
	RefreshloggerSettingsStruct();
}

void CXmlSettings::RefreshVideoSettingsStruct()
{
	log << INFO << "starting VideoSettings import" << endl;
	videoSettings.width = pt.get<int>("settings.video.resolution.width", 800);
	log << INFO << "width: " << videoSettings.width << endl;

	videoSettings.height = pt.get<int>("settings.video.resolution.height", 600);
	log << INFO << "height: " << videoSettings.height<< endl;

	videoSettings.wantedFps = pt.get<int>("settings.video.wantedFps", 60);
	log << INFO << "waned fps: " << videoSettings.wantedFps << endl;

	videoSettings.fullscreen = pt.get<bool>("settings.video.resolution.fullscreen", false);
	log << INFO << "bool fullscreen: " << videoSettings.fullscreen << endl;

	videoSettings.noFrame = pt.get<bool>("settings.video.resolution.noFrame", false);
	log << INFO << "width: " << videoSettings.width << endl;
	log << INFO << "VideoSettings import done" << endl;
}

void CXmlSettings::RefreshAudioSettingsStruct()
{
	log << INFO << "Starting Audio settings import" << endl;
	audioSettings.mute = pt.get<bool>("settings.audio.mute", false);
	log << INFO << "Imported bool mute: " << audioSettings.mute << endl;

	audioSettings.masterVolume = pt.get<float>("settings.audio.volume.master", 1.0);
	log << INFO << "Imported masterVolume: " << audioSettings.masterVolume << endl;

	audioSettings.effectsVolume = static_cast<float>(pt.get("settings.audio.volume.effects", 1.0));
	log << INFO << "Imported effectsVolume: " << audioSettings.effectsVolume << endl;

	audioSettings.musicVolume = static_cast<float>(pt.get("settings.audio.volume.music", 1.0));
	log << INFO << "Imported musicVolume: " << audioSettings.musicVolume << endl;

	audioSettings.ambienceVolume = static_cast<float>(pt.get("settings.audio.volume.ambience", 1.0));
	log << INFO << "Imported ambienceVolume: " << audioSettings.ambienceVolume << endl;
	log << INFO << "Audio settings import done" << endl;
}

void CXmlSettings::RefreshControlSettingsStruct()
{
	log << INFO << "Starting control settings import" << endl;
	controls.moveLeft = static_cast<SDLKey>(pt.get<int>("settings.controls.movement.left", 276));
	log << INFO << "Imported key for: moveLeft (" << controls.moveLeft << ")" << endl;

	controls.moveRight = static_cast<SDLKey>(pt.get<int>("settings.controls.movement.right", 275));
	log << INFO << "Imported key: moveRight (" << controls.moveRight << ")" << endl;

	controls.jump = static_cast<SDLKey>(pt.get<int>("settings.controls.movement.jump", 273));
	log << INFO << "Imported key: jump (" << controls.jump << ")" << endl;

	controls.duck = static_cast<SDLKey>(pt.get<int>("settings.controls.movement.duck", 274));
	log << INFO << "Imported key: duck (" << controls.duck << ")" << endl;

	controls.attack = static_cast<SDLKey>(pt.get<int>("settings.controls.combat.attack", 32));
	log << INFO << "Imported key: attack (" << controls.attack << ")" << endl;

	controls.pause = static_cast<SDLKey>(pt.get<int>("settings.controls.interface.pause", 112));
	log << INFO << "Imported key: pause (" << controls.pause << ")" << endl;

	controls.menu = static_cast<SDLKey>(pt.get<int>("settings.controls.interface.menu", 27));
	log << INFO << "Imported key: menu (" << controls.menu << ")" << endl;

	controls.mute = static_cast<SDLKey>(pt.get<int>("settings.controls.interface.mute", 109));
	log << INFO << "Imported key: menu (" << controls.mute << ")" << endl;

	log << INFO << "Control settings import done" << endl;
}

void CXmlSettings::RefreshMultiplayerSettingsStruct()
{
	log << INFO << "Starting multiplayer settings import" << endl;
	multiPlayerSettings.playerIsClient = pt.get<bool>("settings.multiplayer.playerisClient", false);
	log << INFO << "Imported bool playerIsClient: " << multiPlayerSettings.playerIsClient << endl;

	multiPlayerSettings.opponentIP = pt.get<std::string>("settings.multiplayer.opponentIP", "127.0.0.1");
	log << INFO << "Imported opponent IP address: " << multiPlayerSettings.opponentIP << endl;
	multiPlayerSettings.port = pt.get<int>("settings.multiplayer.port", 2000);
	log << INFO << "Imported port number: " << multiPlayerSettings.port << endl;
	log << INFO << "Multiplayer settings import done" << endl;
}

void CXmlSettings::RefreshloggerSettingsStruct()
{
	log << INFO << "Starting logger settings import" << endl;
	loggerSettings.logToConsole = pt.get<bool>("settings.logging.logToConsole", true);
	loggerSettings.logLevel = static_cast<Level>(pt.get<int>("settings.logging.loglevel", 1));
	log << INFO << "Logger settings import done" << endl;
}
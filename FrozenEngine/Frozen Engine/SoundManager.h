/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   23:04
	filename: 	SoundManager.h
	file path:	FrozenEngine\Frozen Engine
	file base:	SoundManager
	file ext:	h
	author:		Kristian Skarseth
	
	purpose:	Sound manager to play music and sound effects, using
				openAl lib.
*********************************************************************/
#ifndef SoundManager_h__
#define SoundManager_h__

//Disable alut lib depricated warning
#pragma warning (disable : 4996)

// I N C L U D E S

#include <iostream>
#include <string>
#include <map>

#include <al.h>
#include <alc.h>
#include <alut.h>

#include "Logger.h"
#include "xmlResourcesImport.h"
#include "xmlSettingsHandling.h"

using std::string;

// C L A S S  D E C L A R A T I O N

class CSound
{
public:
	static GCPtr<CSound> Inst();
	
	//Play a music file(loop) until stoppped or new music is started;
	void PlayMusic(string MusicName);
	//Play a sound effect
	void PlaySound(string SoundName);
	//Stop all music
	void StopMusic();
	//Mute / Unmute all sound
	void ToggleMute();
	//Change gain for all sources
	void setGlobalGain(float gain);

private:
	CSound(void); //ctor
	~CSound(void); //dtor

	//Print out errors with logger
	void _DisplayOpenALError(char* errorMSG, ALenum error);
	//Set Gain level float (0 - 1)
	void _setGain(float gain){ _gain = gain;}
	//Get gain level
	float _getGain(){return _gain;}

	Logger log;

	// openAL required sound vars
	ALenum _format;
	ALsizei _size;
	ALsizei _freq;
	ALvoid* _data;
	ALboolean _loop;
	ALfloat _gain;

	bool _mute;

	ALsizei _fileNumber;

	//Sound buffer array
	ALuint* _Buffers;
	//Sound source array
	ALuint* _Sources;

	//Sound name map
	std::map<string, ALuint> _alutMap;
};
#endif // SoundManager_h__
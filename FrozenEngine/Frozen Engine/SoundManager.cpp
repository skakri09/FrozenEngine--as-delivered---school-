#include "SoundManager.h"

// P U B L I C  F U N C T I O N S

GCPtr<CSound> CSound::Inst()
{
	static GCPtr<CSound> instance = new CSound();
	return instance;
}

CSound::CSound() : log("SoundManager", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	GCPtr<CXmlResourcesImport> xmlImport = new CXmlResourcesImport(SOUND_FILES, "XMLparser(soundMan)");
	_fileNumber = xmlImport->GetFilePathVector()->size();

	_Buffers = new ALuint[_fileNumber]; 
	_Sources = new ALuint[_fileNumber];

	_setGain(1); //Set default volume.

	alutInit(NULL, 0); //Initialize Alut

	alGetError(); //Check for Errors before load

	//Generate buffers
	alGenBuffers(_fileNumber, _Buffers);

	for(int i = 0; i < _fileNumber; i++)
	{
		const char* filePath= xmlImport->GetFilePathVector()->at(i).c_str();
		log << INFO << "Loading sound file: " << filePath << endl;

		//Load file with alut.
		alutLoadWAVFile(const_cast<char*>(filePath), &_format, &_data, &_size, &_freq, &_loop);
		//Add alut data to buffer
		alBufferData(_Buffers[i], _format, _data, _size, _freq);
		//Clear vars
		alutUnloadWAV(_format, _data, _size, _freq);
	}

	//Generate sources
	alGenSources(_fileNumber, _Sources);

	for(int i = 0; i < _fileNumber; i++)
	{
		alSourcei (_Sources[i], AL_BUFFER, _Buffers[i]);
		alSourcef (_Sources[i], AL_PITCH, 1);
		alSourcef (_Sources[i], AL_GAIN, _gain);

		if (i < 2)
		{
			alSourcei (_Sources[i], AL_LOOPING, AL_TRUE);
		} 
		else
		{
			alSourcei (_Sources[i], AL_LOOPING, AL_FALSE);
		}
	}

	for(int i = 0; i < _fileNumber; i++)
	{
		_alutMap[xmlImport->GetFileNameVector()->at(i)] = _Sources[i];
	}

	if (alGetError() != AL_NO_ERROR) 
	{ 
		_DisplayOpenALError("Load/ALUT_ERROR: ", alGetError()); 
	}
}

CSound::~CSound()
{
	StopMusic();
	//Delete openAL SoundBuffers
	alDeleteBuffers(_fileNumber, _Buffers);
	//Delete openAL SoundSources
	alDeleteSources(_fileNumber, _Sources);
	if(_Buffers)
	{
		delete _Buffers;
		_Buffers = 0;
	}
	if(_Sources)
	{
		delete _Sources;
		_Sources = 0;
	}
	//Close openAL
	alutExit();
}

void CSound::PlayMusic(string MusicName)
{
	//Stop any music currently playing
	StopMusic();

	//Find song in map
	if(_alutMap.find(MusicName) != _alutMap.end())
	{
		//Play song
		alSourcePlay(_alutMap.find(MusicName)->second);
	}
	else
	{
		log << ERRORX << MusicName << " not found." << endl;
	}
}

void CSound::PlaySound(string SoundName)
{
	//Find source in Map
	if(_alutMap.find(SoundName) != _alutMap.end())
	{
		//Play sound
		alSourcePlay(_alutMap.find(SoundName)->second);
	}
	else
	{
		log << ERRORX << SoundName << "sound not found." << endl;
	}
}

void CSound::StopMusic()
{
	//Stop playing all sources
	for (int i = 0; i < _fileNumber; i++)
	{
		alSourceStop(_Sources[i]);
	}
}

void CSound::ToggleMute()
{
	//Set gain to 0 for all sources
	(_getGain() == 1) ? _setGain(0) : _setGain(1);

	for (int i = 0; i < _fileNumber; i++)
	{
		alSourcef(_Sources[i], AL_GAIN, _gain);
	}
}

void CSound::setGlobalGain(float gain)
{
	_setGain(gain);

	for (int i = 0; i < _fileNumber; i++)
	{
		alSourcef(_Sources[i], AL_GAIN, _gain);
	}
}

// P R I V A T E  F U N C T I O N S

void CSound::_DisplayOpenALError(char* errorMSG, ALenum error)
{
	switch(error) 
	{
	case AL_INVALID_NAME:
		log << WARN << "AL_INVALID_NAME: " << errorMSG << std::endl;
		break;
	case AL_INVALID_ENUM:
		log << WARN << "AL_INVALID_ENUM: " << errorMSG << std::endl;
		break;
	case AL_INVALID_VALUE:
		log << WARN << "AL_INVALID_VALUE: " << errorMSG << std::endl;
		break;
	case AL_INVALID_OPERATION:
		log << WARN << "AL_INVALID_OPERATION: " << errorMSG << std::endl;
		break;
	case AL_OUT_OF_MEMORY:
		log << WARN << "AL_OUT_OF_MEMORY: " << errorMSG << std::endl;
		break;
	default:
		log << WARN << "**Unknown AL_ERROR** :" << errorMSG << std::endl;
		break;
	}
}



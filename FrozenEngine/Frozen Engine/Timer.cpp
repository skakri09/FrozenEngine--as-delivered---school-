// I N C L U D E S

#include "Timer.h"

// P U B L I C  F U N C T I O N S

GCPtr<CTimer> CTimer::Inst()
{
	static GCPtr<CTimer> instance = new CTimer();
	return instance;
}

CTimer::CTimer() : log("Timer", WARN)//CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	_UpdatesPerIter = 0;
	_IterDuration = 0;
	_PreviosIterUpdates = 0;
	_CurrentIterUpdates = 0;
	_IterBeginTick = 0;
	_NextIterTick = 0;
	_NextIterBeginTime = 0;

	_LastIterBeginTick = 0;
	_SDLTicks = 0;
	_DeltaTime = 0.0;

	_beginTick = 0;

	_wantedFPS = CXmlSettings::Inst()->GetVideoSettings()->wantedFps;
	_avgFPS = 0.0;
	_FPSPrint = 0;
}

float CTimer::getElapsedTime()
{
	return static_cast<float>(_SDLTicks - _beginTick);
}

// D E R I V E D  F U N C T I O N S

void CTimer::Update()
{
	_CalcDeltaTime(); //Calculate Time since last frame

	_calcFPS(); //Calculate FPS

	_SleepUntilUpdate(); //Sleep
}

bool CTimer::Start()
{
	//Timer stats
	setFPS(_wantedFPS);
	SetIterDuration(1000);

	//Sleep 
	_IterBeginTick = SDL_GetTicks();
	_NextIterTick = _IterBeginTick + _IterDuration;
	_NextIterBeginTime = _IterBeginTick + ((_CurrentIterUpdates + 1) * _IterDuration / _UpdatesPerIter);

	//Delta time
	_SDLTicks = _IterBeginTick;
	_LastIterBeginTick = _SDLTicks;

	//Time
	_beginTick = SDL_GetTicks();

	return true;
}

void CTimer::Stop()
{
}

// P R I V A T E   F U N C T I O N S

void CTimer::_SleepUntilUpdate()
{
	long TicksUntilUpdate = _NextIterBeginTime - SDL_GetTicks();

	if (TicksUntilUpdate > 0)
	{
		SDL_Delay(TicksUntilUpdate);
	}

	// Sleep done - update vars
	++_CurrentIterUpdates;

	_NextIterBeginTime = _IterBeginTick + ((_CurrentIterUpdates + 1) * _IterDuration / _UpdatesPerIter);

	while (_NextIterBeginTime > _NextIterTick)
	{
		_IterBeginTick = _NextIterTick;
		_NextIterTick += _IterDuration;
		_PreviosIterUpdates = _CurrentIterUpdates;
		_CurrentIterUpdates = 0;
	}
}

void CTimer::_calcFPS()
{
	float FPS = 1 / _DeltaTime;

	_FPSVector.push_back(FPS);
	//Return avg. FPS based on the past 10 frames
	if (_FPSVector.size() == 10)
	{
		_avgFPS = 0;

		for(FloatVector::iterator i = _FPSVector.begin(); i != _FPSVector.end(); ++i)
		{
			_avgFPS += (*i);
		}
		_FPSPrint = (static_cast<int>(ceil(_avgFPS / 10.0F)) - 1);
		_FPSVector.clear();
	}
}

void CTimer::_CalcDeltaTime()
{
	_LastIterBeginTick = _SDLTicks;

	_SDLTicks = SDL_GetTicks();

	_DeltaTime = (_SDLTicks - _LastIterBeginTick) / 1000.0F;

	log << INFO << "DeltaTick: " << _DeltaTime << std::endl;
}
/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   19:33
	filename: 	Timer.h
	file path:	FrozenEngine\Frozen Engine
	file base:	Timer
	file ext:	h
	author:		Peder Thorup, Kristian Skarseth
	
	purpose:	Timer class that keeps the track of the game-time for 
				Frame-independant-movment purposes aswell as calling 
				SDL_SLEEP-functionality to save CPU.
				Using Singleton DP
*********************************************************************/

#ifndef Timer_h__
#define Timer_h__

// I N C L U D E S

#include <iostream>
#include <vector>
#include <math.h>

#include <SDL.h>

#include "Logger.h"
#include "Task.h"
#include "GCPtr.h"
#include "xmlSettingsHandling.h"

typedef std::vector<float> FloatVector;

// C L A S S  D E C L A R A T I O N

class CTimer : public Task
{
public:
	void Update();
	bool Start();
	void Stop();

	static GCPtr<CTimer> Inst();

	// S E T T E R S  & G E T T E R S

	// Set number of wanted updates during the time interval.
	void setFPS(int wantedFPS){_UpdatesPerIter = wantedFPS;}
	// Set duration (in millisecs) of the time interval.
	void SetIterDuration( int IntDurInMilliSecs ){_IterDuration = IntDurInMilliSecs;}
	//Returns time since last frame
	float getDeltaTime(){return _DeltaTime;}
	//Returns the total MS since game start
	float getElapsedTime();
	//Returns average FPS
	int getFPSPrint(){ return _FPSPrint; }
	//Returns wanted game FPS (imported from XML)
	int getWantedFPS(){ return _wantedFPS; }

private:
	CTimer();  //ctor hidden
	CTimer(CTimer const&);

	// Sleep until it's time for coming update.
	void _SleepUntilUpdate();
	// Prints out an average FPS based on last 10 frames
	void _calcFPS();
	// Calculates time since last frame
	void _CalcDeltaTime();

	//Timer vars
	int	_UpdatesPerIter;		 // Number of updates we're striving to get per interval.
	int	_IterDuration;			 // Duration of interval in milliseconds.
	int	_PreviosIterUpdates;	 // Number of updates last interval total.
	int	_CurrentIterUpdates;	 // Number of updates current interval so far.				
	int _IterBeginTick;			 // When current interval started.		
	int _NextIterTick;			 // When next interval starts.
	int _NextIterBeginTime;		 // Ideal time for next update.

	//Delta tick vars
	int _LastIterBeginTick;		 //Beginning tick of last update
	int _SDLTicks;			     // Current SDL_Tick
	float _DeltaTime;			 // Time since last update

	//time
	int _beginTick;

	//FPS vars
	FloatVector _FPSVector;
	int _FPSPrint;
	float _avgFPS;

	//Import timer vals
	int _wantedFPS;

	Logger log;
};

#endif // Timer_h__
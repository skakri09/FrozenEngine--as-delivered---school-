/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   19:53
	filename: 	CKernel.h
	file path:	FrozenEngine\Frozen Engine
	file base:	CKernel
	file ext:	h
	author:		Peder Thorup, Kristian Skarseth
	
	purpose:	Works like a taskmanager that keeps track of the current
				tasks. Can also add new tasks and remove them.
*********************************************************************/

#ifndef CKernel_h__
#define CKernel_h__

// I N C L U D E S

#include <iostream>
#include <string>
#include <vector>

#include <SDL.h>

#include "Task.h"
#include "GCPtr.h"
#include "InputManager.h"
#include "Logger.h"
#include "SoundManager.h"
#include "RenderEngine.h"
#include "xmlSettingsHandling.h"

using std::string;
using std::cout;

typedef std::vector<GCPtr<Task>> gameTaskQue;

// C L A S S   D E C L A R A T I O N

class CKernel
{
public:
	static GCPtr<CKernel> Inst(); 
	
	
	// The "heart-pump", looping over and over as long as there are tasks left 
	// in the active-list, running their Update() functions. (Unless the tasks 
	// are flagged for removal/kill: In that case their Stop() functions are 
	// run instead, and they are removed from the list.)
	void Run();
	
	// Killing all tasks (which includes running their Stop() functions), 
	// on next update-loop in Execute.
	void RemoveAllTasks();

	// Adding a task to the active-list and running it's Start().
	bool AddTask(GCPtr<Task> &task);

	// Adding a task to a list of tasks that will be included in update
	// loop on next update. Is required when adding a task to the update-
	// loop from an Update() function as the iterator otherwise will be broken.
	bool AddTaskForNextUpdate(GCPtr<Task> &task);

	// Flagging a task for removal (kill), which will happen on next update. 
	// (And it's Stop() function will be run at that time too.)
	void RemoveTask(GCPtr<Task> &task);

	// Pausing task. (Moving it to the pause-list, running it's Suspend().)
	void PauseTask(GCPtr<Task> &task);

	// Unpausing task. (Moving it to the active-list, running it's Resume().)
	void ResumeTask(GCPtr<Task> &task);

	std::string GetEngineversion(){return _engineVersion;}
private: 
	Logger log;

	CKernel();
	~CKernel();

	bool m_bGameOver;
	bool m_bNewTaskToAdd;
	bool m_bFlaggedForPause;

	bool Init();

	gameTaskQue runningTasks;
	gameTaskQue tasksToAdd;

	std::string _engineVersion;
};

#endif // CKernel_h__
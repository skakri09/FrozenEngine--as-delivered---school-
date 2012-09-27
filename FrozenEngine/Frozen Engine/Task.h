/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   19:58
	filename: 	Task.h
	file path:	FrozenEngine\Frozen Engine
	file base:	Task
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	Interface used by all classes that is added to the 
				task manager.
*********************************************************************/

#ifndef Task_h__
#define Task_h__

// I N C L U D E S

#include "Logger.h"
#include "xmlSettingsHandling.h"
#include "GCPtr.h"

#include <vector>

// C L A S S  D E C L A R A T I O N

class Task
{
public:
	Task(long priority = 5000) : log("Updateables", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
	{
		isNewTask = false;
		pause = false;
		Kill = false;
		Priority = priority;

		log << INFO << "Parent constructor done. " << std::endl;
	}
	virtual ~Task(){log << INFO << "Destructor done." << std::endl;}

	// Retrieves the priority value of this task (lower value = higher priority).
	virtual long GetPriority(){return Priority;}

	// Returns true if task should be stopped, and removed from any task list.
	virtual bool CanKill(){return Kill;}

	// Returns true if task is in a paused state
	virtual bool Paused(){return pause;}

	// Flags task for removal.
	virtual void FlagForKill(){Kill = true;}

	// Unflags a task for removal
	virtual void UnflagForKill(){Kill = false;}

	// Flags the task for pause
	virtual void FlagForPause(){pause = true;}

	// Unflags the task for pause
	virtual void UnflagForPause(){pause = false;}

	// Pure virtual function. Should contain initializing stuff to be done when a task is added to the kernel system.
	virtual bool Start() = 0;

	// Pure virtual function. Should contain cleanup stuff to be done when a task is removed from the kernel system.
	virtual void Stop() = 0;

	// Pure virtual function. Should contain repeating stuff to be done repeatedly in the kernel.
	virtual void Update() = 0;

	// Should be overloaded if we want to do special stuff on a task pause.
	virtual void Suspend(){}

	// Should be overloaded if we want to do special stuff returning from a task pause.
	virtual void Resume(){}
	
	/* Used to run Start() on a task that has been added to the kernel loop		*/
	/* with UnflagForPause, to currently run old Stop()s before new Start()s	*/
	virtual void ClearIsNewTask(){isNewTask = false;}
	virtual void SetIsNewTask(){isNewTask = true;}
	virtual bool IsNewTask(){return isNewTask;}

protected:
	bool isNewTask;
	bool pause;
	bool Kill;
	long Priority;
	static std::vector<Task*> tasks;
private:
	Logger log;
};

#endif // Task_h__
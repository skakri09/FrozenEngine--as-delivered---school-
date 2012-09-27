// I N C L U D E S

#include "Kernel.h"

// P U B L I C  F U N C T I O N S


GCPtr<CKernel> CKernel::Inst()
{
	static GCPtr<CKernel> instance = new CKernel();
	return instance;
}

// SINGLETON
CKernel::CKernel() : log("Manager", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
}

CKernel::~CKernel()
{
}

void CKernel::Run()
{
	//Initializes SDL and other subsystems needed troughout the engine.
	Init();
	
	while(!runningTasks.empty())
	{
		// If we have one or more tasks in the tasksToAdd vector
		// we add them to the runningTasks and call Start() 
		if(!tasksToAdd.empty())
		{
			for(unsigned int i = 0; i < tasksToAdd.size(); i++)
			{
				runningTasks.push_back(tasksToAdd.at(i));
				//runningTasks.back()->Start();
			}
			tasksToAdd.clear();
		}
		for(auto i = runningTasks.begin(); i != runningTasks.end();)
		{
			if((*i)->CanKill())
			{
				(*i)->UnflagForKill();
				(*i)->Stop();
				i = runningTasks.erase(i);
			}
			else if((*i)->Paused())
			{
				//do nothing
				++i;
			}
			
			else
			{
				if((*i)->IsNewTask())
				{
					(*i)->ClearIsNewTask();
					(*i)->Start();
				}
				(*i)->Update();
				++i;
			}
		}
	}
}

bool CKernel::AddTask(GCPtr<Task> &task)
{
	task->UnflagForPause();
	runningTasks.push_back(task);
	return runningTasks.back()->Start();
}

void CKernel::RemoveTask(GCPtr<Task> &task)
{
	for (gameTaskQue::iterator i = runningTasks.begin(); runningTasks.end() != i; ++i)
	{
		if (*i == task)
		{
			(*i)->FlagForKill();
			break;
		}
	}
}

void CKernel::RemoveAllTasks()
{
	for (gameTaskQue::iterator i = runningTasks.begin(); runningTasks.end() != i; i++)
	{
		RemoveTask(*i);
	}
}

void CKernel::PauseTask(GCPtr<Task> &task)
{
	for (gameTaskQue::iterator i = runningTasks.begin(); runningTasks.end() != i; ++i)
	{
		if(*i == task)
		{
			(*i)->FlagForPause();
		}
	}
}

void CKernel::ResumeTask(GCPtr<Task> &task)
{
	for (gameTaskQue::iterator i = runningTasks.begin(); runningTasks.end() != i; ++i)
	{
		if(*i == task)
		{
			(*i)->UnflagForPause();
		}
	}
}

bool CKernel::AddTaskForNextUpdate(GCPtr<Task> &task)
{
	tasksToAdd.push_back(task);
	task->SetIsNewTask();
	return m_bNewTaskToAdd;
}

// P R I V A T E  F U N C T I O N S

bool CKernel::Init()
{
	if(!SDL_WasInit(SDL_INIT_EVERYTHING))
	{
		if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
		{
			log << CRITICAL << "Init failed..." << endl;
			return false;
		}
	}
	return true;
}


// I N C L U D E S

#include <SDL.h>

//#include "Kernel.h"
//#include "SoundManager.h"
//#include "GameManager.h"
//#include "RenderEngine.h"
//#include "Timer.h"

#include "Logger.h"
#include "Menu.h"
#include "StateManager.h"
#include "xmlSettingsHandling.h"
using std::endl;

// M A I N

int main(int argc, char* args[])
{
	Logger log("Main ", INFO);
	log << INFO << "test" << std::endl;

	/* - Creating the state manager - */
	GCPtr<Task> StateManager(new CStateManager());
	/* -Adding it to the update loop- */
	CKernel::Inst()->AddTask(StateManager);
	
	/* Starting the kernel. Once the kernel loop is empty	   */
	/* we are returned here to clean up the rest of the engine */
	CKernel::Inst()->Run();


/************************************************************************/

	/* Collect GCPtrs as a precaution.									*/
	/* This SHOULD be done automaticly when atexit() is called later.	*/
	log << INFO << "GCPtr exit manual collect" << endl;
	GCPtr<int>::collect();


	log << INFO << "Calling SDL_Quit()" << endl;
	SDL_Quit();


	return 0;
}

//End of line :0


//
///* ---- S T A R T I N G    E N G I N E   C O R E ---- */
//
//log << INFO << "Creating Kernel Object" << endl;
////CKernel KernelObj("Frozen Engine 0.401");
//CKernel::Inst();
//
//
//log << INFO << "New RenderEngine Task" << endl;
//GCPtr<Task> RenderEngine(CRenderEngine::Inst());
//
///* displaying a still image as "loading screen" */
//CRenderEngine::Inst()->DisplayStillImage("FrozenLoading800x600");
//
//log << INFO << "New Input Task" << endl;
//GCPtr<Task> InputUpdate(CInput::Inst());
//
//log << INFO << "New Timer Task" << endl;
//GCPtr<Task> TimerUpdate(CTimer::Inst());
//
//CKernel::Inst()->AddTask(RenderEngine);
//CKernel::Inst()->AddTask(InputUpdate);
//CKernel::Inst()->AddTask(TimerUpdate);
//
///* Creating a menu object and adding it to the kernel */
//log << INFO << "New GameMenu Task" << endl;
//GCPtr<Task> GameMenu(new CMenu());
//CKernel::Inst()->AddTask(GameMenu);
//
///* Starting the kernel. Once the kernel loop is empty	   */
///* we are returned here to clean up the rest of the engine */
//CKernel::Inst()->Run();
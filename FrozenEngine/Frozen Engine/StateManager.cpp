#include "StateManager.h"

CStateManager::CStateManager() 
	: log("StateManager", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{

}

CStateManager::~CStateManager()
{

}

bool CStateManager::Start()
{
	Initialize();
	return true;
}

void CStateManager::Stop()
{

}

void CStateManager::Update()
{
	
}

void CStateManager::Initialize()
{
/* ---- S T A R T I N G    E N G I N E   C O R E ---- */
	log << INFO << "Creating Kernel Object" << endl;
	//CKernel KernelObj("Frozen Engine 0.401");
	CKernel::Inst();

	GCPtr<Task> RRender(CRRenderEngine::Inst());
	log << INFO << "New RenderEngine Task" << endl;
	//GCPtr<Task> RenderEngine(CRenderEngine::Inst());
	
	/* displaying a still image as "loading screen" */
	CRRenderEngine::Inst()->DisplayStillImage("FrozenLoading800x600");

	log << INFO << "New Input Task" << endl;
	GCPtr<Task> InputUpdate(CInput::Inst());

	log << INFO << "New Timer Task" << endl;
	GCPtr<Task> TimerUpdate(CTimer::Inst());

	

	CKernel::Inst()->AddTask(RRender);
	//CKernel::Inst()->AddTask(RenderEngine);
	CKernel::Inst()->AddTask(InputUpdate);
	CKernel::Inst()->AddTask(TimerUpdate);

	RunMenu();
}


void CStateManager::RunGame()
{

	CleanupMenu();
}

void CStateManager::RunMenu()
{
	/* Creating a menu object and adding it to the kernel */
	log << INFO << "New GameMenu Task" << endl;
	GCPtr<Task> GameMenu(new CMenu());
	CKernel::Inst()->AddTask(GameMenu);

	CleanupGame();
}


	// P R I V A T E // 
void CStateManager::CleanupGame()
{

}

void CStateManager::CleanupMenu()
{

}




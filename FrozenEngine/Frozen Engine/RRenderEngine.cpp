#include "RRenderEngine.h"

/* Singleton - private Ctor/Dtor */
CRRenderEngine::CRRenderEngine()
	: log("RenderEngine2", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	log << WARN << "HELLO WORLD" << std::endl;

	screenSurface = NULL;
	doRenderUpdates = true;

	position.w = 0;
	position.h = 0;

	baseWidth = CXmlSettings::Inst()->GetVideoSettings()->width;
	baseHeight = CXmlSettings::Inst()->GetVideoSettings()->height;
	bpp = 16;
	flags = SDL_HWSURFACE | SDL_DOUBLEBUF;

	/* Initializing sdl_video if it wasn't already */
	if(!SDL_WasInit(SDL_INIT_VIDEO))
	{
		if(0>SDL_Init(SDL_INIT_VIDEO))
		{
			log << CRITICAL << "Could not initialize SDL video subsystems - " << SDL_GetError() << std::endl;
		}
		else
		{
			/* Initializing the screen surface */
			screenSurface = SDL_SetVideoMode(baseWidth, baseHeight, bpp, flags);
			if(!screenSurface)
			{
				log << CRITICAL << "Cannot set video mode - " << SDL_GetError() << std::endl;
			}
		}
	}
}

CRRenderEngine::~CRRenderEngine()
{
	if(screenSurface)
	{
		SDL_FreeSurface(screenSurface);
		screenSurface = 0;
	}
}

// P U B L I C   F U N C T I O N S

CRRenderEngine* CRRenderEngine::Inst()
{
	static GCPtr<CRRenderEngine> instance = new CRRenderEngine();
	return instance;
}

bool CRRenderEngine::Start()
{

	return true;
}

void CRRenderEngine::Stop()
{
	
	renderVector.clear();
}

void CRRenderEngine::Update()
{
	
	RenderRenderables();
	SDL_Flip(screenSurface);
}

void CRRenderEngine::AddRenderable( CRenderableNoAnimation* renderable )
{
	renderVector.push_back(renderable);
	log << INFO << "Added renderable to renderVector." << std::endl;
}

void CRRenderEngine::RemoveRenderable( CRenderableNoAnimation* renderable )
{
	for(iter = renderVector.begin(); iter != renderVector.end();)
	{
		if( (*iter) == renderable)
		{
			renderVector.erase(iter);
			iter = renderVector.end();
		}
		else
		{
			++iter;
		}
	}
}

void CRRenderEngine::ClearRenderableVector()
{
	renderVector.clear();
}

void CRRenderEngine::PauseAllCurrRenderContent()
{
	doRenderUpdates = false;
}

void CRRenderEngine::UnPauseAllCurrRenderContent()
{
	doRenderUpdates = true;
}

void CRRenderEngine::DisplayStillImage( std::string imageName )
{
	//SDL_Surface* stillImage = NULL;
	//stillImage = CSurfaces::Inst()->GetSurface(imageName);

	SDL_BlitSurface(CSurfaces::Inst()->GetSurface(imageName), NULL, screenSurface, NULL);
	SDL_Flip(screenSurface);
	/*
	if(stillImage)
	{
		delete stillImage;
		stillImage = 0;
	}*/
}


// P R I V A T E   F U N C T I O N S

void CRRenderEngine::RenderRenderables()
{
	if(doRenderUpdates)
	{
		for(iter = renderVector.begin(); iter != renderVector.end(); ++iter)
		{
			if( (*iter)->ShouldRenderSurface() )
			{
				/* Placing the 2D position from the renderable CVector2D, in a SDL_Rect */
				position.x = static_cast<int>( (*iter)->Get2DPosition()->getX() );
				position.y = static_cast<int>( (*iter)->Get2DPosition()->getY() );
				SDL_Rect a = *(*iter)->GetRect();
				/* Blits the surface to the screen, assumes GetSurface returns a real surface */
				SDL_BlitSurface( (*iter)->GetSurface(), (*iter)->GetRect(), screenSurface, &position );
			}
		}
	}
}

void CRRenderEngine::ChangeResolution( int newWidth, int newHeight )
{
	screenSurface = SDL_SetVideoMode(newWidth, newHeight, 0, flags);
}


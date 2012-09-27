#include "RenderEngine.h"

GCPtr<CRenderEngine> CRenderEngine::Inst()
{
	static GCPtr<CRenderEngine> instance = new CRenderEngine();
	return instance;
}

CRenderEngine::CRenderEngine() : log("RenderEngine", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	_screenSurface = 0;

	// Initialize SDL_INIT_VIDEO if not Initialized before.
	if (!SDL_WasInit(SDL_INIT_VIDEO))
	{
		if (0 > SDL_Init(SDL_INIT_VIDEO))
		{
			log << CRITICAL << "Cannot Initialize SDL video Subsystem - " << SDL_GetError() << endl;
		}
		else
		{
			// Create a graphics output screen/window.
			baseWidth = CXmlSettings::Inst()->GetVideoSettings()->width;
			baseHeight = CXmlSettings::Inst()->GetVideoSettings()->height;
			_screenSurface = SDL_SetVideoMode(
				baseWidth,						//Window Width
				baseHeight,						//Window Height
				16,								//Bit depth
				SDL_HWSURFACE | SDL_DOUBLEBUF);	//Flags
			
			if (!_screenSurface) 
			{
				log << CRITICAL << "Cannot set video mode - " << SDL_GetError() << endl;
			}
		}
	}
}

// D E R I V E D  F U N C T I O N S

bool CRenderEngine::Start()
{
	playingVideo = false;
	return true;
}

void CRenderEngine::Stop()
{
	ClearImageList();
}

void CRenderEngine::Update()
{
	if(!playingVideo)
	{
		_RenderListUpdate(); //Render images in vector
	}
	else
	{
		playVideo(currentMovieName); // Play video
	}

	SDL_Flip(_screenSurface); //Update screen;
}

// P U B L I C  F U N C T I O N S

void CRenderEngine::AddRenderImg( CImageObject* obj)
{
	_renderList.push_back(obj);
	log << INFO << "Image added to RenderQueue" <<  std::endl;
}

void CRenderEngine::ClearImageList()
{
	_renderList.clear();
}

void CRenderEngine::StartMovie( std::string movieName )
{
	playVideo(movieName);
}

void CRenderEngine::StopMovie()
{
	playingVideo = false; 
	if(movie.GetStatus() != SMPEG_STOPPED)
	{
		movie.Stop();
	}

	// Resetting the resolution to the base values
	ChangeResolution(baseWidth, baseHeight);
}

void CRenderEngine::DisplayStillImage(std::string imageName)
{
	SDL_Surface* stillImage = NULL;
	stillImage = CSurfaces::Inst()->GetSurface(imageName);

	SDL_BlitSurface(stillImage, NULL, _screenSurface, NULL);
	SDL_Flip(_screenSurface);
	
	if(stillImage)
	{
		stillImage = 0;
	}
}

void CRenderEngine::ChangeResolution(int newWidth, int newHeight)
{
	_screenSurface = SDL_SetVideoMode(
		newWidth,						//Window Width
		newHeight,						//Window Height
		16,								//Bit depth
		SDL_HWSURFACE | SDL_DOUBLEBUF);	//Flags
}

void CRenderEngine::playVideo(std::string movieName)
{
	if(!playingVideo)
	{	
		if(movie.Load( movieName, _screenSurface, 1, 1 ))
		{
			playingVideo = true;
			VideoInfo resolution = *movie.GetVideoInfo();
			int movieWidth = resolution.width;
			int movieHeight = resolution.height;
			ChangeResolution(movieWidth, movieHeight);
			movie.Play();
		}
	}
	else
	{
		if(CInput::Inst()->KeyDown(SDLK_ESCAPE))
		{
			playingVideo = false;
		}
		if( (movie.GetStatus() == SMPEG_PLAYING) && playingVideo == true)
		{
			SDL_FillRect( _screenSurface, 0, 0 );
			movie.Display();
		}
		else
		{
			StopMovie();
		}
	}
}

// P R I V A T E  F U N C T I O N S

void CRenderEngine::_RenderListUpdate()
{
	for(RenderList::iterator i = _renderList.begin(); i != _renderList.end();)
	{
		if((**i).RemoveImage())
		{
			i = _renderList.erase(i);
		}
		else
		{
			rect.x = static_cast<int>((**i).getPositionX());
			rect.y = static_cast<int>((**i).getPositionY());

			crop.x = (**i).getCropX();
			crop.y = (**i).getCropY();

			crop.w = (**i).getImageWidth();
			crop.h = (**i).getImageHeight ();

			SDL_BlitSurface((**i).getImageSurface(), &crop, _screenSurface, &rect);

			(**i).UpdateAnimation();
			
			++i;
		}
	}
}
#include "Surfaces.h"

// P U B L I C   F U N C T I O N S

GCPtr<CSurfaces> CSurfaces::Inst()
{
	static GCPtr<CSurfaces> instance = new CSurfaces();
	return instance;
}

CSurfaces::CSurfaces() : log("Surfaces", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{ 
	Init();
}

CSurfaces::~CSurfaces()
{
	_FreeAllSurfaces();
}

void CSurfaces::Init()
{
	XMLImport = new CXmlResourcesImport(SURFACE_FILES, "resourceImport(surfaces) ");
	InitSurfaces();
	InitSprites();
}

void CSurfaces::InitSurfaces()
{
	int importSize =  XMLImport->GetFilePathVector()->size();

	for(int i = 0; i < importSize; i++)
	{
		AddSurfaceToMap(XMLImport->GetFileNameVector()->at(i), LoadImage(XMLImport->GetFilePathVector()->at(i)));
	}
	SDL_Surface* no_surface = NULL;

	AddSurfaceToMap("no_surface", no_surface);
}

void CSurfaces::InitSprites()
{
	int importSize =  XMLImport->GetSpriteFilePathVector()->size();
	for(int i = 0; i < importSize; i++)
	{
		SpriteSurface* sprite = new SpriteSurface;
		std::string spriteName = XMLImport->GetSpriteFileNameVector()->at(i);
		sprite->spriteInformation = XMLImport->GetSpriteInfoVector()->at(i);
		sprite->theSurface = LoadImage(XMLImport->GetSpriteFilePathVector()->at(i));

		AddSpriteSurfaceToMap(spriteName, sprite);
	}
}

SDL_Surface* CSurfaces::GetSurface(std::string surfaceName)
{
	SDL_Surface* theSurface = 0; 

	if( SurfaceMap.find(surfaceName) != SurfaceMap.end() )
	{
		theSurface = SurfaceMap.find(surfaceName)->second;
	}
	else
	{
		if( SurfaceMap.find("bad_image") != SurfaceMap.end() )
		{
			theSurface = SurfaceMap.find("bad_image")->second;
			log << ERRORX << "Surface(" << surfaceName << ") could not be found, using 'bad_image'" << endl;
		}
		else
		{
			log << ERRORX << "Surface(" << surfaceName << ") could not be found" << endl;
		}
	}
	return theSurface;
}

SpriteSurface* CSurfaces::GetSprite( std::string spriteName )
{
	SpriteSurface* theSprite = new SpriteSurface;
	if(SpriteMap.find(spriteName) != SpriteMap.end())
	{
		theSprite = SpriteMap.find(spriteName)->second;
		log << INFO << "found sprite " << spriteName << endl;
	}
	else
	{
		if( SurfaceMap.find("bad_image") != SurfaceMap.end() )
		{
			theSprite->theSurface = SurfaceMap.find("bad_image")->second;
			log << ERRORX << "sprite(" << spriteName << ") could not be found, using 'bad_image'" << endl;
		}
		else
		{
			log << ERRORX << "Surface(" << spriteName << ") could not be found" << endl;
		}
	}
	return theSprite;
}

void CSurfaces::ResizeSurfaces(float resizeVal)
{
	for(SurfacePointerMap::iterator i = SurfaceMap.begin(); i != SurfaceMap.end(); i++)
	{
		i->second = rotozoomSurface(i->second, 0, resizeVal, 1);
	}
}

// P R I V A T E   F U N C T I O N S

SDL_Surface* CSurfaces::LoadImage(std::string filename)
{
	SDL_Surface* loadedImage = 0;
	SDL_Surface* optimizedImage = 0;

	loadedImage = IMG_Load( filename.c_str() );	

	if(!loadedImage) 
	{
		log << WARN << "File not Found: " << filename << endl;;
	}
	else
	{
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		SDL_FreeSurface( loadedImage );
	}
	return optimizedImage;
}

SDL_Surface* CSurfaces::LoadImage(std::string filename, float resizeFactor)
{
	SDL_Surface* returnSurface = 0;
	returnSurface = LoadImage(filename);
	returnSurface = rotozoomSurface(returnSurface, 0, resizeFactor, 1);
	return returnSurface;
}

void CSurfaces::AddSurfaceToMap(string surfaceKeyName, SDL_Surface* surface)
{
	SurfaceMap[surfaceKeyName] = surface;
	log << INFO << "Added " << surfaceKeyName << " to map" << endl;
}

void CSurfaces::AddSpriteSurfaceToMap( string spriteKeyName, SpriteSurface* spriteInfo )
{
	SpriteMap[spriteKeyName] = spriteInfo;
	log << INFO << "Added " << spriteKeyName << " to map" << endl;
}

void CSurfaces::_FreeAllSurfaces()
{
	SurfacePointerMap::iterator surfIter;
	SpriteSurfacePointerMap::iterator spriteIter;

	for(surfIter = SurfaceMap.begin(); surfIter != SurfaceMap.end(); surfIter++)
	{
		log << INFO << "Surface free: " << (*surfIter).first << endl;
		SDL_FreeSurface((*surfIter).second);
		(*surfIter).second = 0;
	}

	for(spriteIter = SpriteMap.begin(); spriteIter != SpriteMap.end(); spriteIter++)
	{
		log << INFO << "Surface free: " << (*surfIter).first << endl;
		SDL_FreeSurface((*spriteIter).second->theSurface);
		(*spriteIter).second->theSurface = 0;
		delete (*spriteIter).second;
		(*spriteIter).second = 0;
	}
}
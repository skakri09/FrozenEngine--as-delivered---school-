/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   19:50
	filename: 	Surfaces.h
	file path:	FrozenEngine\Frozen Engine
	file base:	Surfaces
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	Load in all surfaces defined in XML into surface objects
				and places them in a searchable stl map.
				Using Singleton DP
*********************************************************************/

#ifndef Surfaces_h__
#define Surfaces_h__

// I N C L U D E S

#include <string>
#include <map>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <string>

#include "xmlResourcesImport.h"
#include "GCPtr.h"
#include "Logger.h"
#include "SDL_rotozoom.h"
#include "xmlSettingsHandling.h"

using std::map;
using std::vector;
using std::endl;

struct SpriteSurface
{
	SDL_Surface* theSurface;
	SpriteInfo* spriteInformation;
};

typedef std::map<std::string, SDL_Surface*> SurfacePointerMap;
typedef std::map<std::string, SpriteSurface*> SpriteSurfacePointerMap;

// C L A S S  D E C L A R A T I O N

class CSurfaces
{
public:
	//static CSurfaces* Inst();
	static GCPtr<CSurfaces> Inst();

	// Returns the spesificed (search-name) surface from a map with all surfaces
	SDL_Surface* GetSurface(std::string surfaceName);
	SpriteSurface* GetSprite(std::string spriteName);

	void ResizeSurfaces(float resizeVal);

protected:
private:
	CSurfaces();
	~CSurfaces();
	
	//static CSurfaces* instance;
	
	GCPtr<CXmlResourcesImport> XMLImport;
	void Init();
	void InitSurfaces();
	void InitSprites();

	Logger log;

	// Loads an image based upon the filename
	SDL_Surface* LoadImage(std::string filename);

	// Overloaded LoadImage function, will resize the surface when loading it, 
	// using rotozoom and the desired resizeFactor
	SDL_Surface* LoadImage(std::string filename, float resizeFactor);

	// Frees all dynamically allocated surfaces
	void _FreeAllSurfaces();

	// Adds the param surfaceKeyName and surface pointer to our surfacesMap
	void AddSurfaceToMap(string surfaceKeyName, SDL_Surface* surface);
	
	void AddSpriteSurfaceToMap(string spriteKeyName, SpriteSurface* spriteInfo);

	SurfacePointerMap SurfaceMap; // Map for normal surfaces
	SpriteSurfacePointerMap SpriteMap; // Map for sprites
};

#endif // Surfaces_h__
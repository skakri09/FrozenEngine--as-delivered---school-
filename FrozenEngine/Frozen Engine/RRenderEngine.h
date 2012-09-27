/********************************************************************
*	Created:	2012/04/20  12:19
*	Filename: 	RRenderEngine.h
*	Author:		Kristian Skarseth
*	
*	purpose:	Render surfaces
*********************************************************************/
#ifndef RRenderEngine_H
#define RRenderEngine_H

#include "Logger.h"
#include "Task.h"
#include "xmlSettingsHandling.h"
#include "RenderableNoAnimation.h"
#include "Surfaces.h"

#include <vector>
#include <SDL.h>
#include <string>

class CRenderableNoAnimation; /* forward declaration */ 

class CRRenderEngine : public Task
{
public:
	static CRRenderEngine* Inst();
	
	/* Inherited Task functionality */
	bool Start();
	void Stop();
	void Update();	

	/* Adds a renderable object to the vector of renderables that we render. */
	void AddRenderable(CRenderableNoAnimation* renderable);
	
	/* Removes a renderable object from the renderables vector.	*/
	void RemoveRenderable(CRenderableNoAnimation* renderable);

	/* Empties the vector of renderables, stopping all current rendering */
	void ClearRenderableVector();

	/* Pauses all current rendering */
	void PauseAllCurrRenderContent();

	/* Unpauses all current rendering */
	void UnPauseAllCurrRenderContent();

	/* Displays a single still image on the screen surface. The image will  */
	/* be overwritten as soon as anything else is blitted to the screen.	*/
	void DisplayStillImage(std::string imageName);

	void ChangeResolution(int newWidth, int newHeight);

private:
	/* Hidden Ctor/Dtor as this is a singleton. */
	CRRenderEngine();
	~CRRenderEngine(); 

	/* Loops trough the renderVector and blits all the renderables to screen */
	void RenderRenderables();

	/* Our render vector where pointers to CRenderable objects are placed. */
	/* The vector is being looped trough once every game update, and the   */
	/* Surfaces in the Renderable objects are being blitted to the screen. */
	std::vector<CRenderableNoAnimation*> renderVector;
	std::vector<CRenderableNoAnimation*>::iterator iter; /* Iterator for vector above. */

	Logger log; /* Logger object */

	SDL_Rect position; /* SDL_Rect holding position on the screen we're rendering too */	

	SDL_Surface* screenSurface; /* The screen surface that we are rendering to */

	bool doRenderUpdates; /* When true, we wont loop the renderVector.		 */
	bool movieIsPlaying;  /* True if the renderEngine is displaying a movie. */
	
	int baseWidth;	/* The base screen width imported from XML on startup.	*/
	int baseHeight; /* The base screen height imported from XML on startup. */
	int bpp;		/* The bits per pixel we want to init sdl video with	*/

	Uint32 flags; /* The flags we set in SDL_SetVideoMode */

};
#endif // RRenderEngine_H

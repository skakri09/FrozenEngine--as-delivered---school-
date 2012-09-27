/********************************************************************
*	Created:	2012/04/20  10:51
*	Filename: 	Renderable.h
*	Author:		Kristian Skarseth
*	
*	purpose:	Provides a surface object the functionality required to
				render it with a SDL_Surface*. The class uses the render-
				engine to render the images.
*********************************************************************/
#ifndef Renderable_H
#define Renderable_H

#include "xmlSettingsHandling.h"
#include "SurfaceInterface.h"
#include "RRenderEngine.h"
#include "Logger.h"
#include "Surfaces.h"

#include <string>


class CRenderableNoAnimation :  public virtual CSurfaceInterface
{
public:
	/* Ctor used for normal single image initialization. */
	CRenderableNoAnimation(std::string surfaceName, bool renderSurface = true);
	/* Ctor used by CRenderableAnimation for a sprite initialization. */
	/* Information is set manually by CRenderableAnimation.			  */
	CRenderableNoAnimation();
	virtual ~CRenderableNoAnimation();

	/* Inherited from the surfaceInterface */
	virtual void UpdateSurface();

	/* Returns if the surface is set to be rendered or not. */
	bool ShouldRenderSurface();

	/* Starts rendering the surface. If it was allready being rendered, */
	/* no changes are applied. This func only changes a bool value      */
	void ResumeRenderingSurface();
	
	/* Stops rendering the surface. If the surface was not rendering     */
	/* prior to call, no changes are applied. It only changes a bool val */
	void StopRenderingSurface();

	/* Returns a SDL_Surface* to the surface this obj is using */
	SDL_Surface* GetSurface();

	/* Manually sets the surface by it's name  */
	void SetSurface(std::string surfaceName);

	/* Manually sets the surface by a surface* */
	void SetSurface(SDL_Surface* theSurf);

	int GetImageWidth();
	int GetImageHeight();

	int GetXCrop();
	void SetXCrop(int val);

	int GetYCrop();
	void SetYCrop(int val);

protected:
	bool render;	/* True when we're rendering the surface */
	SDL_Surface* theSurface; /* The objects surface */

private:
	Logger log; /* Our logger */

};
#endif // Renderable_H

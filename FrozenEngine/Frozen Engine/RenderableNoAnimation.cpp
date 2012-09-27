#include "RenderableNoAnimation.h"

CRenderableNoAnimation::CRenderableNoAnimation( std::string surfaceName, bool renderSurface)
	: log("Renderable", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	// 1: Find surfaceName in surface factory
	// 2: Add surface to renderEngine
	theSurface = NULL;
	theSurface = CSurfaces::Inst()->GetSurface(surfaceName);
	CSurfaceInterface::SetRect(theSurface->clip_rect);
	render = renderSurface;
	CSurfaceInterface::surfaceRect = theSurface->clip_rect;
	CRRenderEngine::Inst()->AddRenderable(this);
}

CRenderableNoAnimation::CRenderableNoAnimation()
	: log("Renderable", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	theSurface = NULL;

	render = true;
	CRRenderEngine::Inst()->AddRenderable(this);
}

CRenderableNoAnimation::~CRenderableNoAnimation()
{
	// Remove surface from render engine.
	// Must happen THIS update, so we dont crash
	// on next update when the object does not exist.
	CRRenderEngine::Inst()->RemoveRenderable(this);
}

void CRenderableNoAnimation::UpdateSurface()
{

}

void CRenderableNoAnimation::ResumeRenderingSurface()
{
	render = true;
}

void CRenderableNoAnimation::StopRenderingSurface()
{
	render = false;
}

SDL_Surface* CRenderableNoAnimation::GetSurface()
{
	return theSurface;
}

bool CRenderableNoAnimation::ShouldRenderSurface()
{
	return render;
}

void CRenderableNoAnimation::SetSurface( std::string surfaceName)
{
		theSurface = CSurfaces::Inst()->GetSurface(surfaceName);
}

void CRenderableNoAnimation::SetSurface( SDL_Surface* theSurf )
{
	theSurface = theSurf;
	surfaceRect = theSurf->clip_rect;
}

int CRenderableNoAnimation::GetImageWidth()
{
	return static_cast<int>(surfaceRect.w);
}

int CRenderableNoAnimation::GetImageHeight()
{
	return static_cast<int>(surfaceRect.h);
}

int CRenderableNoAnimation::GetXCrop()
{
	return CSurfaceInterface::surfaceRect.x;
}

void CRenderableNoAnimation::SetXCrop( int val )
{
	CSurfaceInterface::surfaceRect.x = val;
}

int CRenderableNoAnimation::GetYCrop()
{
	return CSurfaceInterface::surfaceRect.y;
}

void CRenderableNoAnimation::SetYCrop( int val )
{
	CSurfaceInterface::surfaceRect.y = val;
}


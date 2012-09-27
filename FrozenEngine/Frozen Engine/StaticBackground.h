#ifndef StaticBackground_H
#define StaticBackground_H

#include "RenderableNoAnimation.h"
#include "xmlSettingsHandling.h"
#include "Logger.h"

class CStaticBackground : public CRenderableNoAnimation
{
public:
	CStaticBackground(std::string surfaceName);
	~CStaticBackground();

	void UpdateSurface();

private:
	Logger log;
};
#endif // StaticBackground_H

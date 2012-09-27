#ifndef MovingBackground_H
#define MovingBackground_H

#include "Movable.h"
#include "RenderableNoAnimation.h"
#include "xmlSettingsHandling.h"
#include "Logger.h"

class CMovingBackground : public CMovable,
						  public CRenderableNoAnimation
{
public:
	CMovingBackground();
	~CMovingBackground();
	
	void UpdateSurface();

private:
	Logger log;

};
#endif // MovingBackground_H

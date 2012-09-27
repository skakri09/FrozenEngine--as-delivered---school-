#ifndef MovingInteractableEnvironment_H
#define MovingInteractableEnvironment_H

#include "Interactable.h"
#include "RenderableNoAnimation.h"
#include "Movable.h"
#include "xmlSettingsHandling.h"
#include "Logger.h"

class CMovingInteractableEnvironment : public CInteractable,
									   public CRenderableNoAnimation,
									   public CMovable
{
public:
	CMovingInteractableEnvironment();
	~CMovingInteractableEnvironment();

	void UpdateSurface();

private:
	Logger log;

};
#endif // MovingInteractableEnvironment_H

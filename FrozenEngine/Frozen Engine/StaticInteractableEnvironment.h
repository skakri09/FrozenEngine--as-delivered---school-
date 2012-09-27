#ifndef StaticInteractableEnvironment_H
#define StaticInteractableEnvironment_H

#include "Interactable.h"
#include "RenderableNoAnimation.h"
#include "xmlSettingsHandling.h"
#include "Logger.h"

class CStaticInteractableEnvironment : public CInteractable,
									   public CRenderableNoAnimation
{
public:
	CStaticInteractableEnvironment();
	~CStaticInteractableEnvironment();

	void UpdateSurface();

private:
	Logger log;

};




#endif // StaticInteractableEnvironment_H

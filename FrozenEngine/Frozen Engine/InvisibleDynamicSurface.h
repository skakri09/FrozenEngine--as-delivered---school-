#ifndef InvisibleDynamicSurface_H
#define InvisibleDynamicSurface_H

#include "Interactable.h"
#include "Movable.h"
#include "Logger.h"
#include "xmlSettingsHandling.h"

class CInvisibleDynamicSurface : public CInteractable,
								 public CMovable
{
public:
	CInvisibleDynamicSurface();
	virtual ~CInvisibleDynamicSurface();

	void UpdateSurface();

private:
	Logger log;

};
#endif // InvisibleDynamicSurface_H

#ifndef InvisibleStaticSurface_H
#define InvisibleStaticSurface_H

#include "Interactable.h"
#include "Logger.h"
#include "xmlSettingsHandling.h"

class CInvisibleStaticSurface : public CInteractable
{
public:
	CInvisibleStaticSurface();
	~CInvisibleStaticSurface();

	void UpdateSurface();

private:
	Logger log;

};
#endif // InvisibleStaticSurface_H

#ifndef BaseCharacter_H
#define BaseCharacter_H

#include "RenderableAnimation.h"
#include "RenderableNoAnimation.h"
#include "Movable.h"
#include "Interactable.h"
#include "Logger.h"
#include "Gravity.h"

class CBaseCharacter :  public CRenderableAnimation, public CGravity,
						public CInteractable
{
public:
	CBaseCharacter(std::string spriteName);
	virtual ~CBaseCharacter();

	virtual void UpdateSurface();

	void MoveLeft(bool run = false);
	void MoveRight(bool run = false);
	void Jump();

	void SetHP();
	void SetDmg();

	void UpdatePosition();

private:
	Logger log;
};
#endif // BaseCharacter_H

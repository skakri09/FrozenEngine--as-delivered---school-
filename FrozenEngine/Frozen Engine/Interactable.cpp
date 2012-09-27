#include "Interactable.h"

CInteractable::CInteractable() : log("Interactable", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	// nothing to construct
}

CInteractable::~CInteractable()
{
	// nothing to destruct
}

bool CInteractable::IsRectCollision( SDL_Rect& otherRect )
{
	if(		(otherRect.x >	position2d.getX()					) 
		&&	((otherRect.x + otherRect.w) < (position2d.getX() + surfaceRect.w	) ) 
		&&	(otherRect.y >	position2d.getY()					) 
		&&	((otherRect.y + otherRect.h) < (position2d.getY() + surfaceRect.h	) ) )
	{
		return true;
	}
	else return false;
}

bool CInteractable::IsInteractableCollision( CInteractable* otherInteratable )
{
	if(		(otherInteratable->Get2DPosition()->getX() > position2d.getX() ) 
		&&	((otherInteratable->Get2DPosition()->getX() + otherInteratable->GetRect()->w) 
				< (position2d.getX() + surfaceRect.w	) ) 
		&&	(otherInteratable->Get2DPosition()->getY() > position2d.getY() ) 
		&&	((otherInteratable->Get2DPosition()->getY() + otherInteratable->GetRect()->h) 
				< (position2d.getY() + surfaceRect.h	) ) )
	{
		return true;
	}
	else return false;
}

bool CInteractable::IsMouseCollision()
{
	mouseCoords mousePos = CInput::Inst()->getMousePos();
	if(mousePos.x > position2d.getX()
		&& mousePos.x < (position2d.getX() + surfaceRect.w)
		&& mousePos.y > position2d.getY()
		&& mousePos.y < (position2d.getY() + surfaceRect.h) )
	{
		return true;
	}
	else return false;
}

bool CInteractable::IsClickable()
{
	return isClickable;
}

void CInteractable::MakeClickable()
{
	isClickable = true;
}

void CInteractable::MakeUnClickable()
{
	isClickable = false;
}

bool CInteractable::IsCollidable()
{
	return isCollidable;
}

void CInteractable::MakeCollidable()
{
	isCollidable = true;
}

void CInteractable::MakeUnCollidable()
{
	isCollidable = false;
}

void CInteractable::UpdateSurface()
{

}




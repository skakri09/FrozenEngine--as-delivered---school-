/********************************************************************
	created:	2012/04/18
	created:	18:4:2012   16:04
	filename: 	SurfaceInterface.h
	file path:	\FrozenEngine\Frozen Engine
	file base:	SurfaceInterface
	file ext:	h
	author:		Kristian Skarseth
	
	Purpose:	A base "interface" (not a 'real' interface as we have two variables)
				for all surfaces in the engine.

				The class only contains what absolutely every surface object requires.
				It is however an abstract class, and cannot be used to instantiate 
				an object on it's own. The 4 sub classes inheriting from this interface 
				are to be used when creating a surface. They can be combined to build
				advanced surfaces (like a player character), or used alone to create
				a simple static background image.

	Inheriting classes to create a surface from:----------------------------------------|
																						|
				CMovable		/* Provides functionality to move a surface				|
																						|
				CInteractable	/* Provides functionality to interact with a surface	|
																						|
				CAnimationable	/* Provides functionality to animate a surface.			|
								/* The surface will typically be a sprite in that case.	|
																						|
				CRenderable		/* Provides functionality to render a surface.			|
*****************************************************************************************/

#ifndef SurfaceInterface_h__
#define SurfaceInterface_h__

#include <SDL.h>		/* SDL_rect		*/
#include "Vector2D.h"	/* 2D position	*/

/* Typedef for signed __int16. Using it to cast when */
/* setting SDL_Rect x/y to avoid compiler warning.	 */
typedef signed __int16	Sint16;

class CSurfaceInterface
{
public:
	CSurfaceInterface(){}			// ctor()
	virtual ~CSurfaceInterface(){};	// dtor()

	/* Returns the 2D position of the object as a pointer to the CVector2D object.		*/
	virtual CVector2D* Get2DPosition()	{ return &position2d; }
	/* Returns the X position of the object as a float									*/
	virtual float GetXPosition()		{ return position2d.getX(); }
	/* Returns the Y position of the object as a float									*/
	virtual float GetYPosition()		{ return position2d.getY(); }

	/* Set the 2d position using a CVector2D object */
	virtual void SetPosition(CVector2D newPosition) { position2d = newPosition; }
	/* Set the X position using a float	*/
	virtual void SetXPosition(float newXPosition)	{ position2d.setX(newXPosition); }
	/* Set the Y position using a float	*/
	virtual void SetYPosition(float newYPosition)	{ position2d.setY(newYPosition); }

	/* Returns the rect(rectangle) of the object as a pointer to the SDL_rect object.	*/
	virtual SDL_Rect* GetRect()			{ return &surfaceRect; }

	/* Pure virtual function to change a surface, that any surface object will have.	*/
	virtual void UpdateSurface() = 0;

	/* Sets the rect we're rendering to the screen */
	virtual void SetRect(SDL_Rect& newRect) { surfaceRect = newRect; }
protected:
	
	CVector2D position2d;	/* The 2D position of the object stored in a CVector2D object	*/
	
	SDL_Rect surfaceRect;	/* The Rect of the object, stored in a SDL_Rect object			*/

};

#endif // SurfaceInterface_h__

/********************************************************************
*	Created:	2012/04/18  21:19
*	Filename: 	Interactable.h
*	Author:		Kristian Skarseth
*	
*	purpose:	Add functionality to interact with a surface. The class
				will allow the user to toggle the surface clickable/
				collidable (whether or not you can collide with it), 
				as well as add various different collision check functions.
*********************************************************************/
#ifndef Interactable_H
#define Interactable_H

#include <SDL.h>				 /* SDL_Rect		*/
#include "Logger.h"				 /* Logger			*/
#include "SurfaceInterface.h"	 /* Our interface	*/
#include "xmlSettingsHandling.h" /* Settings import	*/
#include "InputManager.h"

class CInteractable :  public virtual CSurfaceInterface
{
public:
	CInteractable();		  /* ctor */
	virtual ~CInteractable(); /* dtor */
	
	/* Inherited from the surfaceInterface */
	virtual void UpdateSurface();

	/*		---			C O L L I S I O N 		---			  */
	/* If the obj is clickable, we can click it. If clickable */
	
	bool IsRectCollision(SDL_Rect& otherRect);
	bool IsInteractableCollision(CInteractable* otherInteratable);
	bool IsMouseCollision();

	/*		---			C L I C K A B L E		---			  */
	/* If the obj is clickable, we can click it. If clickable */
	/* is set to false, the obj will not be clickable.		  */

	bool IsClickable();		/* Check if the obj is clickable  */
	void MakeClickable();	/* Make the obj clickable		  */
	void MakeUnClickable(); /* Make the obj un-clickable	  */

	
	/*		---			C O L L I D A B L E			---		   */
	/* If the obj is collidable, we can check collision on it. */
	/* If collidable is set to false, collision will not apply.*/

	bool IsCollidable();	 /* Check if the obj is collidable */
	void MakeCollidable();	 /* Make the obj collidable		   */
	void MakeUnCollidable(); /* Make the obj un-collidable	   */

protected:
	bool isClickable;	/* True when you can click the obj		  */
	bool isCollidable;	/* True when you can collide with the obj */

private:
	Logger log; /* Our logger */

};
#endif // Interactable_H

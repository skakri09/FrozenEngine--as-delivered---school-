/********************************************************************
*	Created:	2012/04/18  19:28
*	Filename: 	Movable.h
*	Author:		Kristian Skarseth
*	
*	purpose:	The class adds functionality to move a surface in 2D
				space. It inherits from the surface 'interface', 
				CSurfaceInterface, and any class inheriting from this 
				class will be able to move a SDL_Rect surface around.
*********************************************************************/

#ifndef Movable_H
#define Movable_H

#include "xmlSettingsHandling.h"/* Setting logger settings				*/
#include "SurfaceInterface.h"	/* The interface we're inheriting from	*/
#include "Vector2D.h"			/* Velocity and Acceleration			*/
#include "Logger.h"				/* Logger								*/
#include "Timer.h"

class CMovable :  public virtual CSurfaceInterface 
{
public:
	CMovable();
	virtual ~CMovable();

	/* Inherited from CSurfaceInterface */
	virtual void UpdateSurface();

	void UpdatePosition(CVector2D* pos, CVector2D* Vel);
	/*		---		 V E L O C I T Y		---			  */
	/*----------------------------------------------------*/
	/** Set the velocity of the obj using a CVector2D obj */
	void SetVelocity(CVector2D newVelocity);
	/** Get the velocity of the obj in CVector2D format   */
	CVector2D* GetVelocity();
	
	/** Set the X velocity of the object with a float	  */
	void SetXVelocity(float newXVelocity);
	/** Get the X velocity of the object as a float		  */
	float GetXVelocity();
	
	/** Set the Y velocity of the object with a float	  */
	void SetYVelocity(float newYVelocity); 
	/** Get the Y velocity of the object as a float		  */
	float GetYVelocity();

	
	
	/*		---		 A C C E L E R A T I O N		---		  */
	/*--------------------------------------------------------*/
	/** Set the acceleration of the obj using a CVector2D obj */
	void SetForce(CVector2D newAcceleration);
	/** Get the acceleration of the obj in CVector2D format   */
	CVector2D* GetForce();

	/** Set the X acceleration of the object with a float	 */
	void SetXForce(float newXAcceleration);
	/** Get the X acceleration of the object as a float		 */
	float GetXForce();

	/** Set the Y acceleration of the object with a float	 */
	void SetYForce(float newYAcceleration);
	/** Get the Y acceleration of the object as a float		 */
	float GetYForce();

protected:
	CVector2D velocity;	/* The velocity of the obj in the form of a Vector2D	 */
	CVector2D force;	/* The force of the obj in the form of a Vector2D		 */

private:
	Logger log;	/* Our local logger object */
};
#endif // Movable_H

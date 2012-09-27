/********************************************************************
*	Created:	2012/04/20  11:45
*	Filename: 	Gravity.h
*	Author:		Kristian Skarseth
*	
*	purpose:	Gravity for surfaces. Any surface object that wants
				gravity, should inherit from this class.
*********************************************************************/
#ifndef Gravity_H
#define Gravity_H

#include "Vector2D.h"
#include "Timer.h"
class CGravity
{
public:
	CGravity(float yGravity = 1000, float xGravity = 0);
	virtual ~CGravity();

	void ApplyGravitation(CVector2D* targetVelocity);
	
	/* Change the gravity using another CVector2D object */
	void ChangeGravity(CVector2D newGravity);

	/* Change the x gravity using a float value */
	void ChangeXGravity(float newXGravity); 

	/* Change the y gravity using a float value */
	void ChangeYGravity(float newYGravity); 

protected:
	CVector2D gravity;
};

#endif // Gravity_H

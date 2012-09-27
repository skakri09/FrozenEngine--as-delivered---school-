#include "Gravity.h"


CGravity::CGravity( float yGravity /*= 1000*/, float xGravity /*= 0*/ )
{
	gravity.setX(xGravity);
	gravity.setY(yGravity);
}

CGravity::~CGravity()
{
	// nothing to destruct
}

void CGravity::ChangeGravity( CVector2D newGravity )
{
	gravity = newGravity;
}

void CGravity::ChangeXGravity( float newXGravity )
{
	gravity.setX(newXGravity);
}

void CGravity::ChangeYGravity( float newYGravity )
{
	gravity.setY(newYGravity);
}

void CGravity::ApplyGravitation( CVector2D* targetVelocity)
{
	*targetVelocity += (gravity * CTimer::Inst()->getDeltaTime());
}

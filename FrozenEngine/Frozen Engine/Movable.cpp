#include "Movable.h"

CMovable::CMovable() : log("Movable", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	// nothing to construct
}

CMovable::~CMovable()
{
	// nothing to delete
}

void CMovable::SetVelocity( CVector2D newVelocity )
{
	velocity = newVelocity;
}

CVector2D* CMovable::GetVelocity()
{
	return &velocity;
}

void CMovable::SetXVelocity( float newXVelocity )
{
	velocity.setX(newXVelocity);
}

float CMovable::GetXVelocity()
{
	return velocity.getX();
}

void CMovable::SetYVelocity( float newYVelocity )
{
	velocity.setY(newYVelocity);
}

float CMovable::GetYVelocity()
{
	return velocity.getY();
}

void CMovable::SetForce( CVector2D newAcceleration )
{
	force = newAcceleration;
}

CVector2D* CMovable::GetForce()
{
	return &force;
}

void CMovable::SetXForce( float newXAcceleration )
{
	force.setX(newXAcceleration);
}

float CMovable::GetXForce()
{
	return force.getX();
}

void CMovable::SetYForce( float newYAcceleration )
{
	force.setY(newYAcceleration);
}

float CMovable::GetYForce()
{
	return force.getY();
}

void CMovable::UpdateSurface()
{

}

void CMovable::UpdatePosition( CVector2D* pos, CVector2D* Vel )
{
	*pos += (*Vel * CTimer::Inst()->getDeltaTime());
}





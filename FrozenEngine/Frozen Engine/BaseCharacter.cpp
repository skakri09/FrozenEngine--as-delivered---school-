#include "BaseCharacter.h"


CBaseCharacter::CBaseCharacter(std::string spriteName)
	: log("BaseCharacter", CXmlSettings::Inst()->GetLoggerSettings()->logLevel),
	CRenderableAnimation(spriteName),
	CGravity(),
	CInteractable()
{

}

CBaseCharacter::~CBaseCharacter()
{

}

void CBaseCharacter::UpdateSurface()
{
	CRenderableAnimation::UpdateSurface();
	CGravity::ApplyGravitation(&velocity);
	CMovable::UpdatePosition(&position2d, &velocity);
}

void CBaseCharacter::MoveLeft( bool run /*= false*/ )
{
	run == true ? CMovable::SetXVelocity(-(CMovable::GetXForce()*2))
		: CMovable::SetXVelocity(-CMovable::GetXForce() );
}

void CBaseCharacter::MoveRight( bool run /*= false*/ )
{
	run == true ? CMovable::SetXVelocity((CMovable::GetXForce()*2))
		: CMovable::SetXVelocity(CMovable::GetXForce() );
}

void CBaseCharacter::Jump()
{
	CMovable::SetYVelocity(-CMovable::GetYForce());
}

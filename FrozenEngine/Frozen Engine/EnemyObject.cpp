#include "EnemyObject.h"

// P U B L I C  F U N C T I O N S

CEnemyObject::CEnemyObject( 
	std::string surfaceName,
	float PositionX /*= 0.0F*/, 
	float PositionY /*= 0.0F*/,
	int HP /*= 1*/,
	int DMG /*= 1*/,
	float RunForce /*= 250*/,
	float JumpForce /*= 450 */) 
	: CAnimationObject(surfaceName, PositionX, PositionY),
	 CSimpleAI(10, 10, 750, 500),
	 log("EnemyObject", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	setEnemyStats(HP, DMG, RunForce, JumpForce);
	CSimpleAI::setEnemySize(CImageObject::getImageWidth(), CImageObject::getImageHeight());
}

CEnemyObject::~CEnemyObject()
{
	// Nothing to delete
}

void CEnemyObject::setEnemyStats( 
	int HP /*= 1*/, 
	int DMG /*= 1*/,
	float RunForce /*= 250*/,
	float JumpForce /*= 450*/ )
{
	_hp = HP;
	_dmg = DMG;
	_movementForce.setX(RunForce);
	_movementForce.setY(JumpForce);
	CImageObject::_velocity.setX(getRunForce());
}


void CEnemyObject::UpdateEnemy(CVector2D& gravity, float deltaTics)
{
	if(CSimpleAI::TurnArround(CImageObject::_imagePos))
	{
		CImageObject::_velocity.InvertX();
	}
	if(CSimpleAI::JumpCharacter())
	{
		CImageObject::setVelocityY(-getJumpForce());
	}


	CImageObject::ApplyGravitation(gravity, deltaTics);

	CImageObject::UpdatePosition(deltaTics);
	
	groundCollision();
}

void CEnemyObject::groundCollision()
{
	if((getPositionY() > 600.0F - getImageHeight()))
	{
		setPositionY(600.0F - getImageHeight());
		setVelocityY(0);
	}
}




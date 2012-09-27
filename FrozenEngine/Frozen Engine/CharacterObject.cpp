#include "CharacterObject.h"

// P U B L I C  F U N C T I O N S

CCharacterObject::CCharacterObject( 
	std::string surfaceName,
	std::string playerName/* = "noname"*/,
	float PositionX /*= 0.0F*/, 
	float PositionY /*= 0.0F*/,
	int HP /*= 1*/,
	int DMG /*= 1*/,
	float RunForce /*= 250*/,
	float JumpForce /*= 450 */)
	: CAnimationObject(surfaceName, PositionX, PositionY)
{
	_playerName = playerName;
	setCharacterStats(HP, DMG, RunForce, JumpForce);
}

CCharacterObject::~CCharacterObject()
{
	// Nothing to delete
}

void CCharacterObject::setCharacterStats( 
	int HP /*= 1*/,
	int DMG /*= 1*/,
	float RunForce /*= 250*/,
	float JumpForce /*= 450*/ )
{
	_hp = HP;
	_dmg = DMG;
	_runForce = RunForce;
	_jumpForce = JumpForce;
}



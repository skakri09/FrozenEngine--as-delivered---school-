/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   23:09
	filename: 	CharacterObject.h
	file path:	FrozenEngine\Frozen Engine
	file base:	CharacterObject
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	A super simple Character class to create a player character.
				
*********************************************************************/
#ifndef CharacterObject_h__
#define CharacterObject_h__

// I N C L U D E S
#include <iostream>
#include <string>

#include "AnimationObject.h"

// C L A S S  D E C L A R A T I O N

class CCharacterObject : public CAnimationObject
{
public:
	CCharacterObject(
		std::string surfaceName,
		std::string playerName = "noname",
		float PositionX = 0.0F, 
		float PositionY = 0.0F, 
		int HP = 1,
		int DMG = 1,
		float RunForce = 250,
		float JumpForce = 450
		);

	~CCharacterObject();

	// S E T T E R S  &  G E T T E R S

	//Quickly set all character stats. Provides default values if some are left out.
	void setCharacterStats(int HP = 1, int DMG = 1, float RunForce = 250, float JumpForce = 450);

	int getHp() const { return _hp; }
	void setHp(int val) { _hp = val; }

	int getDmg() const { return _dmg; }
	void setDmg(int val) { _dmg = val; }

	float getRunForce() const { return _runForce; }
	void setRunForce(float val) { _runForce = val; }

	float getJumpForce() const { return _jumpForce; }
	void setJumpForce(float val) { _jumpForce = val; }

	std::string GetPlayerName(){return _playerName;}
protected:
	int _hp;
	int _dmg;

	std::string _playerName; // The name of the character in this obj
	float _runForce;
	float _jumpForce;

private:
};

#endif // CharacterObject_h__

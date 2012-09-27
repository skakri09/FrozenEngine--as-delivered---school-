/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   23:08
	filename: 	EnemyObject.h
	file path:	FrozenEngine\Frozen Engine
	file base:	EnemyObject
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	A super simple enemy class to create enemy NPCs. Currently
				inherits from a SimpleAi class to controll the enemy.
*********************************************************************/
#ifndef EnemyObject_h__
#define EnemyObject_h__

// I N C L U D E S

#include <iostream>
#include <string>
#include "xmlSettingsHandling.h"
#include "AnimationObject.h"
#include "SimpleAI.h"
#include "Logger.h"

// C L A S S  D E C L A R A T I O N


class CEnemyObject : public CAnimationObject, public CSimpleAI
{
public:
	CEnemyObject(
		std::string surfaceName,
		float PositionX = 0.0F, 
		float PositionY = 0.0F, 
		int HP = 1,
		int DMG = 1,
		float RunForce = 250,
		float JumpForce = 450
		);

	~CEnemyObject();

	void UpdateEnemy(CVector2D& gravity, float deltaTics);

protected:
	void setEnemyStats(int HP = 1, int DMG = 1, float RunForce = 250, float JumpForce = 450);
	void groundCollision();
	int getHp() { return _hp; }
	void setHp(int val) { _hp = val; }

	int getDmg() { return _dmg; }
	void setDmg(int val) { _dmg = val; }

	float getRunForce() { return _movementForce.getX();}
	void setRunForce(float val) { _movementForce.setX(val);}

	float getJumpForce() { return _movementForce.getY(); }
	void setJumpForce(float val) { _movementForce.setY(val); }
	
	int _hp;
	int _dmg;

	CVector2D _movementForce;
	
private:
	Logger log;
};

#endif // EnemyObject_h__

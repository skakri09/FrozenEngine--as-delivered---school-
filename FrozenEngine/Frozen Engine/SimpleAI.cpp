#include "SimpleAI.h"

CSimpleAI::CSimpleAI(
	int areaMinx, 
	int areaMiny, 
	int areaMaxx, 
	int areaMaxy) : log("simpleAi", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	movementArea.x = areaMinx;
	movementArea.y = areaMiny;
	movementArea.h = areaMaxy;
	movementArea.w = areaMaxx;
	direction = AI_RIGHT;
}

CSimpleAI::~CSimpleAI()
{
	// Nothing to delete
}

void CSimpleAI::Jump()
{
	int jump = randomNumberGenerator.MakeRandomNumber(1, 180);
	if(jump == 5)
	{
		//run jump callback
	}

}

void CSimpleAI::UpdateCharacter()
{

}

bool CSimpleAI::JumpCharacter()
{
	int jump = randomNumberGenerator.MakeRandomNumber(1, 200);
	if(jump == 5)
	{
		return true;
	}
	return false;
}

bool CSimpleAI::TurnArround(CVector2D& position)
{

	if(direction == AI_RIGHT)
	{
		if(position.getX() >= movementArea.w - _enemyWidth)
		{
			direction = AI_LEFT;
			log << INFO << "TURNING LEFT "<< endl;
			return true;
		}
	}
	
	if(direction == AI_LEFT)
	{
		if(position.getX() <= movementArea.x)
		{
			direction = AI_RIGHT;
			log << INFO << "TURNING RIGHT" << endl;
			return true;
		}
	}
	/*
	
	if(position.getX() < movementArea.x				||
		position.getX() > movementArea.w - _enemyWidth	||
		position.getY() < movementArea.y				||
		position.getY() > movementArea.h + _enemyHeight )
	{
		log << INFO << "turning arround" << endl;
		log << INFO << position.getX() << endl;
		log << INFO << position.getY() << endl;
		//log << INFO <<movementArea.x	 << endl;
		return true;
	}*/
	//if(position.getX() < movementArea.x){log << INFO << "1" << endl;}
	//
	//if(position.getX() > movementArea.w - _enemyWidth){log << INFO << "2" << endl;}
	//if(position.getY() < movementArea.y){log << INFO << "3" << endl;}
	//if(position.getY() > movementArea.h - _enemyHeight){log << INFO << "4" << endl;}
	////log << INFO << position.getY() << endl;
	//	log << INFO << position.getY() << ",  " << movementArea.h << ",  " << _enemyHeight << endl;
	////log << INFO << position.getX() << endl;
	////log << INFO << position.getX() << endl;
	return false;
}

void CSimpleAI::setEnemySize( int enemyWidth, int enemyHeight )
{
	_enemyWidth = enemyWidth;
	_enemyHeight = enemyHeight;
}

/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   17:53
	filename: 	SimpleAI.h
	file path:	Engine\Frozen Engine
	file base:	SimpleAI
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	An AI class to act as a super simple AI. It will walk a 
				direction with a given velocity untill it hit's the end 
				off the screen, at which point it turns arround and walk
				the other way. Any animation object can in theory use 
				this class.
	
	note:		This is an abstract class. It cannot be used to create 
				an object. It must be used for inheritance.
*********************************************************************/
#ifndef SimpleAI_h__
#define SimpleAI_h__

// I N C L U D E S
#include "RandomNumberGenerator.h"
#include "Vector2D.h"
#include "Logger.h"
#include "xmlSettingsHandling.h"

#include <SDL.h>

// C L A S S  D E C L A R A T I O N
using std::endl;
enum eAiDirection{AI_LEFT, AI_RIGHT};
class CSimpleAI
{
public:
	CSimpleAI(
		int areaMinx, 
		int areaMiny, 
		int areaMaxx, 
		int areaMaxy
		);
	virtual ~CSimpleAI();

	void UpdateCharacter();
	
	/* UNFINISHED. Callback function in parameter that will set jump velocity */
	virtual void Jump();

	bool JumpCharacter();
	bool TurnArround(CVector2D& position);
protected:
	// The rectangle the user of the AI can move within
	SDL_Rect movementArea;
	
	void setEnemySize(int enemyWidth, int enemyHeight);

	int _enemyWidth, _enemyHeight;

	eAiDirection direction;
private:
	Logger log;

	CRandomNumberGenerator randomNumberGenerator;
	
};

#endif // SimpleAI_h__

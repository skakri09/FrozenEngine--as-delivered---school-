/********************************************************************
*	Created:	2012/04/20  11:43
*	Filename: 	TestPlayer.h
*	Author:		Kristian Skarseth
*	
*	purpose:	A test player class. Might evolve to the real player class
				later. Currently used to test the multiple inheritance from
				the different surface handeling classes.
*********************************************************************/
#ifndef TestPlayer_H
#define TestPlayer_H

#include "BaseCharacter.h"
#include "Logger.h"

#include <string>

class CTestPlayer : public CBaseCharacter
					
{
public:
	CTestPlayer(std::string spriteName = "naruto", 
				std::string playerName = "noname",
				float startPosX = 0.0F,
				float startPosY = 0.0F,
				float runForce = 250.0F,
				float jumpForce = 450.0F,
				int playerHP = 100,
				int playerDMG = 10
				);
	~CTestPlayer();

	// Perform all updates on the player object. Should do something
	// with most of the classes we inherit from, like update position,
	// apply gravity, update animation, check for collision(?) etc.
	void UpdateSurface();

	
private:
	Logger log;

	std::string playerName;
	int playerHP;
	int playerDMG;
};

#endif // TestPlayer_H
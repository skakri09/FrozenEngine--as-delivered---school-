#include "TestPlayer.h"

CTestPlayer::CTestPlayer(
	std::string spriteName/* = "naruto"*/, 
	std::string playerName/* = "noname"*/,
	float startPosX/* = 0.0F*/,
	float startPosY/* = 0.0F*/,
	float runForce/* = 250.0F*/,
	float jumpForce/* = 450.0F*/,
	int playerHP/* = 100*/,
	int playerDMG/* = 10*/)
	:	log("TestPlayer", CXmlSettings::Inst()->GetLoggerSettings()->logLevel),
	CBaseCharacter(spriteName)
{
	CMovable::SetXVelocity(0);
	CMovable::SetYVelocity(0);
	CMovable::SetXForce(runForce);
	CMovable::SetYForce(jumpForce);
	SetXPosition(200);
	SetYPosition(200);
}

CTestPlayer::~CTestPlayer()
{

}

void CTestPlayer::UpdateSurface()
{
	CBaseCharacter::UpdateSurface();
	// player only updates if any
} 


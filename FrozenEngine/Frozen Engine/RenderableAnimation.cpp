#include "RenderableAnimation.h"

CRenderableAnimation::CRenderableAnimation(std::string spriteName)
	: log("Animationable", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	if(!PrepSpriteSurface( CSurfaces::Inst()->GetSprite(spriteName) ))
	{
		log << ERRORX << "Sprite was not created successfully." << std::endl;
	}
	ResetAnimationToFirstFrame();
}

CRenderableAnimation::~CRenderableAnimation()
{

}

void CRenderableAnimation::UpdateSurface()
{
	animDirection = DetermineDirection();
	currentAnimState = DetermineAnimationState();

	CRenderableNoAnimation::SetYCrop(GetCorrectYCrop());
	
	DetermineAnimationFrame();
}

void CRenderableAnimation::SetSpriteInfo()
{

}

/* P R I V A T E    F U N C T I O N S */
/*------------------------------------*/

bool CRenderableAnimation::PrepSpriteSurface(SpriteSurface* sprite_info)
{
	spriteInfo = sprite_info->spriteInformation; /* Store the struct pointer locally */

	/* Sets the surface in base class for render */
	CRenderableNoAnimation::SetSurface(sprite_info->theSurface);

	animDirection = RIGHT;
	currentAnimState = IDLE;

	if(spriteInfo > 0 && theSurface > 0)
	{
		return true;
	}	else return false;
}

void CRenderableAnimation::DetermineAnimationFrame()
{
	if ( currentframe  < FramesInAnim() )
	{
		float totalRunTimeSec = (CTimer::Inst()->getElapsedTime() / 1000.0F);
		float timePerFrame = TimePerFrame();
		float animDuration = AnimDuration();
		int timesPlayed = static_cast<int>( floor(totalRunTimeSec / animDuration) );

		float currentAnimTime = totalRunTimeSec - (animDuration * static_cast<float>(timesPlayed));

		currentframe = static_cast<int>(ceil(currentAnimTime / timePerFrame)) - 1;

		CRenderableNoAnimation::SetXCrop(FrameWidth() * currentframe);
	}
	else
	{
		ResetAnimationToFirstFrame();
	}
}

void CRenderableAnimation::ResetAnimationToFirstFrame()
{
	currentframe = 0;
	CRenderableNoAnimation::SetXCrop(0);
}

float CRenderableAnimation::TimePerFrame()
{
	switch(currentAnimState)
	{
	case IDLE: return (spriteInfo->runAnimDuration / spriteInfo->runFrames);//placeholder for idle duration
	case WALKING: return (spriteInfo->runAnimDuration / spriteInfo->runFrames); //placeholder for walking duration
	case RUNNING: return (spriteInfo->runAnimDuration / spriteInfo->runFrames);
	case JUMPING: return (spriteInfo->runAnimDuration / spriteInfo->runFrames); //placeholder for jumping duration
	case FALLING: return (spriteInfo->runAnimDuration / spriteInfo->runFrames); //placeholder for jumping duration
	default: return 1.0F;
	}
}

float CRenderableAnimation::AnimDuration()
{
	switch(currentAnimState)
	{
	case IDLE: return spriteInfo->runAnimDuration;//placeholder for idle duration
	case WALKING: return spriteInfo->runAnimDuration;//placeholder for idle duration
	case RUNNING: return spriteInfo->runAnimDuration;
	case JUMPING: return spriteInfo->runAnimDuration;//placeholder for idle duration
	case FALLING: return spriteInfo->runAnimDuration;//placeholder for idle duration
	default: return 1.0F;
	}
}

int CRenderableAnimation::FramesInAnim()
{
	switch(currentAnimState)
	{
	case IDLE: return spriteInfo->runFrames;//placeholder for idle frames
	case WALKING: return spriteInfo->runFrames;//placeholder for idle frames
	case RUNNING: return spriteInfo->runFrames;
	case JUMPING: return spriteInfo->runFrames;//placeholder for idle frames
	case FALLING: return spriteInfo->runFrames;//placeholder for idle frames
	default: return 1;
	}
}

int CRenderableAnimation::FrameWidth()
{
	switch(currentAnimState)
	{
	case IDLE: return spriteInfo->runFrameWidth;//placeholder for idle width
	case WALKING: return spriteInfo->runFrameWidth;//placeholder for idle width
	case RUNNING: return spriteInfo->runFrames;
	case JUMPING: return spriteInfo->runFrameWidth;//placeholder for idle width
	case FALLING: return spriteInfo->runFrameWidth;//placeholder for idle width
	default: return 1;
	}
}

eAnimationState CRenderableAnimation::DetermineAnimationState()
{
	float XVel = CMovable::GetXVelocity();
	float YVel = CMovable::GetYVelocity();
	if (YVel != 0 || XVel != 0)
	{
		//only yvel
		if (YVel != 0 && XVel == 0)
		{
			// x = 0, pos y
			if (YVel > 0)
			{
				return FALLING;
			}
			// x = 0, neg y
			if (YVel < 0)
			{
				return JUMPING;
			}
		}
		//only xvel
		if (YVel == 0 && XVel != 0)
		{
			//pos x, y = 0
			if (XVel > 0)
			{
				return WALKING;
			}
			//neg x, y = 0
			if (XVel < 0)
			{
				return WALKING;
			}
		}
		//xvel and yvel
		if (YVel != 0 && XVel != 0)
		{
			//positive x and y
			if (YVel > 0 && XVel > 0)
			{
				return WALKING;
			}
			//neg x, pos y
			if (YVel > 0 && XVel < 0)
			{
				return WALKING;
			}
			//neg y, pos x
			if (YVel < 0 && XVel > 0)
			{
				return WALKING;
			}
			//neg x and y
			if (YVel < 0 && XVel < 0)
			{
				return WALKING;
			}
		}
	}
	else /* no velocity, aka standing still */
	{
		return IDLE;
	}
	return IDLE;
}

eDirection CRenderableAnimation::DetermineDirection()
{
	if(CMovable::GetXVelocity() > 0)
	{
		return RIGHT;
	}
	else if(CMovable::GetXVelocity() < 0)
	{
		return LEFT;
	}
	else
	{
		return animDirection;
	}
}

int CRenderableAnimation::GetCorrectYCrop()
{
	switch(currentAnimState)
	{
	case IDLE: return spriteInfo->runFrameHeight;//placeholder for idle height
	case WALKING: return spriteInfo->runFrameHeight;//placeholder for WALKING height
	case RUNNING: return spriteInfo->runFrameHeight;
	case JUMPING: return spriteInfo->runFrameHeight;//placeholder for JUMPING height
	case FALLING: return spriteInfo->runFrameHeight;//placeholder for FALLING height
	default: return 1;
	}
}


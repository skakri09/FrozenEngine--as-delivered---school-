#include "AnimationObject.h"

// P U B L I C  F U N C T I O N S

CAnimationObject::CAnimationObject( 
	std::string spriteName,
	float PositionX /*= 0*/, 
	float PositionY /*= 0*/,
	int cropX /*= 0*/, 
	int cropY /*= 0*/)
	: CImageObject(PositionX, PositionY, cropX, cropY), 
	log("AnimObject", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	CAnimationObject::setImageSurface(CSurfaces::Inst()->GetSprite(spriteName));
	CImageObject::_surfaceName = spriteName;

	ResetAnimationToFirstFrame();
	UpdateAnimation();
	_totalAnimFrames = 0;

	_eAnimationState = STOP;

	_isDirectionLeft = false;
	currentframe = 0;
}

CAnimationObject::~CAnimationObject()
{
	//Nothing to delete
}

void CAnimationObject::setImageSurface( SpriteSurface* spriteSurface )
{
	CImageObject::setImageSurface(spriteSurface->theSurface);
	_spriteInfo = *spriteSurface->spriteInformation;
}

void CAnimationObject::DetermineAnimationFrame()
{
	if (currentframe  < (_spriteInfo.runFrames))
	{
		float totalRunTimeSec = (CTimer::Inst()->getElapsedTime() / 1000.0F);
		float timePerFrame = _spriteInfo.runAnimDuration / static_cast<float>(_spriteInfo.runFrames);
		int timesPlayed = static_cast<int>(floor(totalRunTimeSec / getRunAnimDuration()));

		float currentAnimTime = totalRunTimeSec - (getRunAnimDuration() * static_cast<float>(timesPlayed));
		
		currentframe = static_cast<int>(ceil(currentAnimTime / timePerFrame)) - 1;

		CImageObject::setCropX(_spriteInfo.runFrameWidth * currentframe);
	}
	else
	{
		ResetAnimationToFirstFrame();
	}
}

void CAnimationObject::UpdateAnimation()
{
	if (_velocity.getX() > 0)
	{
		_isDirectionLeft = false;
	}
	else if(_velocity.getX() < 0)
	{
		_isDirectionLeft = true;
	}

	_eAnimationState = determineObjectState();

	if(_eAnimationState == JUMPING_LEFT  ||
	   _eAnimationState == JUMPING_RIGHT ||
	   _eAnimationState == FALLING_LEFT	 ||
	   _eAnimationState == FALLING_RIGHT)
	{
		
		updateJumpAnimation();
	}
	if(_eAnimationState == WALKSPEED_RIGHT	||
			_eAnimationState == WALKSPEED_LEFT)
	{
		updateRunAnimation();
	}
	if (_eAnimationState == STOP)
	{
		setImageWidth(_spriteInfo.runFrameWidth);
		setImageHeight(_spriteInfo.runFrameHeight);
		if(_isDirectionLeft)
		{
			setCropY(_spriteInfo.runFrameHeight);
		}
		else
		{
			setCropY(0);
		}
		ResetAnimationToFirstFrame();
		//log << INFO << "anim stopped" << endl;
	}
	//log << INFO << "cropX: " << getCropX() << ", CropY: " << getCropY() << endl;
}

void CAnimationObject::updateRunAnimation()
{
	setImageWidth(_spriteInfo.runFrameWidth);
	setImageHeight(_spriteInfo.runFrameHeight);
	if (_eAnimationState == WALKSPEED_LEFT)
	{
		setCropY(_spriteInfo.runFrameHeight); 
		DetermineAnimationFrame();
	}
	else if (_eAnimationState == WALKSPEED_RIGHT)
	{
		setCropY(0); 
		DetermineAnimationFrame();
	}
}

void CAnimationObject::updateJumpAnimation()
{
	setImageWidth(_spriteInfo.jumpFrameWidth);
	setImageHeight(_spriteInfo.jumpFrameHeight);
	if (_eAnimationState == JUMPING_RIGHT)
	{
		setCropX(0);
		setCropY(_spriteInfo.runFrameHeight*2); 
	}
	else if (_eAnimationState == JUMPING_LEFT)
	{
		setCropX(0);
		setCropY(_spriteInfo.runFrameHeight*2 + _spriteInfo.jumpFrameHeight);// 
	}
	else if (_eAnimationState == FALLING_RIGHT)
	{
		setCropX(_spriteInfo.jumpFrameWidth);
		setCropY(_spriteInfo.runFrameHeight*2);
	}
	else if (_eAnimationState == FALLING_LEFT)
	{
		setCropX(_spriteInfo.jumpFrameWidth);
		setCropY(_spriteInfo.runFrameHeight*2 + _spriteInfo.jumpFrameHeight);
	}
	//log << INFO << "cropX: " << getCropX() <<", cropY: " << getCropY() << endl;
	
}

void CAnimationObject::ResetAnimationToFirstFrame()
{
	currentframe = 0;
	CImageObject::setCropX(0);
}

eeeAnimationState CAnimationObject::determineObjectState()
{
	if (CImageObject::_velocity.getY() != 0 || CImageObject::_velocity.getX() != 0)
	{
		//only yvel
		if (CImageObject::_velocity.getY() != 0 && CImageObject::_velocity.getX() == 0)
		{
			// x = 0, pos y
			if (CImageObject::_velocity.getY() > 0)
			{
				if (!_isDirectionLeft)
				{
					return FALLING_RIGHT;
				}
				if (_isDirectionLeft)
				{
					return FALLING_LEFT;
				}
			}
			// x = 0, neg y
			if (CImageObject::_velocity.getY() < 0)
			{
				if (!_isDirectionLeft)
				{
					return JUMPING_RIGHT;
				}
				if (_isDirectionLeft)
				{
					return JUMPING_LEFT;
				}
			}
		}
		//only xvel
		if (CImageObject::_velocity.getY() == 0 && CImageObject::_velocity.getX() != 0)
		{
			//pos x, y = 0
			if (CImageObject::_velocity.getX() > 0)
			{
				return WALKSPEED_RIGHT;
			}
			//neg x, y = 0
			if (CImageObject::_velocity.getX() < 0)
			{
				return WALKSPEED_LEFT;
			}
		}
		//xvel and yvel
		if (CImageObject::_velocity.getY() != 0 && CImageObject::_velocity.getX() != 0)
		{
			//positive x and y
			if (CImageObject::_velocity.getY() > 0 && CImageObject::_velocity.getX() > 0)
			{
				return FALLING_RIGHT;
			}
			//neg x, pos y
			if (CImageObject::_velocity.getY() > 0 && CImageObject::_velocity.getX() < 0)
			{
				return FALLING_LEFT;
			}
			//neg y, pos x
			if (CImageObject::_velocity.getY() < 0 && CImageObject::_velocity.getX() > 0)
			{
				return JUMPING_RIGHT;
			}
			//neg x and y
			if (CImageObject::_velocity.getY() < 0 && CImageObject::_velocity.getX() < 0)
			{
				return JUMPING_LEFT;
			}
		}
	}
	//no vel
	else
	{
		return STOP;
	}
	return STOP;
}

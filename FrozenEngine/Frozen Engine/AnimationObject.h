/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   23:10
	filename: 	Frozen Engine\AnimationObject.h
	file path:	FrozenEngine\Frozen Engine
	file base:	AnimationObject
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	AnimationObject inherits from ImageBase, and adds 
				animation functionality to a SDL_Surface* sprite. 
*********************************************************************/
#ifndef AnimationObject_h__
#define AnimationObject_h__

// I N C L U D E S

#include <SDL.h>
#include <string>

#include "ImageObject.h"
#include "Timer.h"
#include "Logger.h"

// C L A S S  D E C L A R A T I O N
enum eeeAnimationState{	WALKSPEED_LEFT, WALKSPEED_RIGHT, 
						RUNSPEED_LEFT, RUNSPEED_RIGHT,
						JUMPING_LEFT, JUMPING_RIGHT, 
						FALLING_LEFT, FALLING_RIGHT,
						STOP
					};
using std::endl;
class CAnimationObject : public CImageObject
{
public:
	CAnimationObject(
		std::string surfaceName,
		float PositionX = 0, float PositionY = 0, 
		int cropX = 0, int cropY = 0
		);
	
	virtual ~CAnimationObject();

	void setImageSurface(SpriteSurface* spriteSurface); // Derived and overloaded
	
	// Updates the animation based on it's animationState,
	// which is determined by the velocity of the obj 
	void UpdateAnimation(); //Derived 

	/* Returns the current animation state of this class */
	eeeAnimationState GetAnimState() { return _eAnimationState; }

private:
	// Plays the correct animation frame for UpdateAnimation()
	void DetermineAnimationFrame();

	Logger log;

	eeeAnimationState determineObjectState();
	float getRunAnimDuration(){return _spriteInfo.runAnimDuration;} /* Returns duration of the run animation */
	void setRunAnimDuration(float val);							    /* Sets the duration of the run animation*/
	void ResetAnimationToFirstFrame();	 /* Set animationFrame to the first frame						*/
	void updateRunAnimation();
	void updateJumpAnimation();

protected:

	int _CalcAnimationFrames();

	eeeAnimationState _eAnimationState; /* The current animation state of this object */

	SpriteInfo _spriteInfo;			/* Struct holding information we need regarding the sprite surface we hold	*/
	
	int currentframe;	/* The frame we're currently on in the animation.							*/
	int _totalAnimFrames;	/* The total number of frames in the animation currently running.			*/
	bool _AnimationStopped; /* True if the animation is not running, same as AnimationState STOP		*/
	bool _isDirectionLeft;
private:
};

#endif // AnimationObject_h__

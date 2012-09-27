/********************************************************************
*	Created:	2012/04/20  11:14
*	Filename: 	Animationable.h
*	Author:		Kristian Skarseth
*	
	IMPORTANT:	A class inheriting form this class should not inherit from 
				CRenderable as well. This class inherits from CRenderable on
				it's own, and overrides the functions required to animate
				the object.

	purpose:	Provides a surface object the functionality required 
				to animate the surface. A surface object inheriting
				this class should have a sprite sheet matching the
				requirements of this class as far as placement of 
				animation frames goes. Details on this can be found below.

	Frames placement:
				Each animation state should have two sets of frames, one
				for each direction. The frames should be organized so that
				each animation state uses two horizontal lines for their 
				frames. The frames for the RIGHT direction on first line, then
				the frames for the LEFT direction on the second line. The 
				frames must be put in correct order (first frame of the animation
				furthest to the left, then ascending to the last frame on the right).

				// more instructions...
*********************************************************************/
#ifndef Animationable_H
#define Animationable_H

#include "Logger.h"
#include "Timer.h"	// delta time
#include "SurfaceInterface.h"
#include "xmlSettingsHandling.h"
#include "RenderableNoAnimation.h"
#include "Movable.h"

/* Current animation state. We can only be */
/* in one animation state at any one time. */
enum eAnimationState {IDLE,WALKING,
					  RUNNING,JUMPING,
					  FALLING,ATTACK_1};

/* Current direction. Direction is used together */
/* with animation state. Each state has a set of */
/* animation frames for each direction.			 */
enum eDirection	{LEFT, RIGHT};

class CRenderableAnimation : public CRenderableNoAnimation,
							 public CMovable
{
public:
	CRenderableAnimation(std::string spriteName = "");
	virtual ~CRenderableAnimation();

	void SetSpriteInfo();
	void UpdateSurface();

protected:

private:
	Logger log;				/* Our logger */
	int currentframe;		/* Frame number in current animation. */
	int totalAnimFrames;	/* Total frames in current animation. */
	eAnimationState currentAnimState; /* Current animation state  */
	eDirection animDirection;
	int walkSpeed, runSpeed;

	/* Calculates and prepares a sprite surface for use. */
	bool PrepSpriteSurface(SpriteSurface* sprite_info);

	/* Determines the animation state, based on the velocity */
	eAnimationState DetermineAnimationState();
	
	/* Determines the direction, based on the velocity		 */
	eDirection DetermineDirection();

	/* Determines which frame to use from the sprite */
	void DetermineAnimationFrame();

	/* Resets the current animation to the first frame */
	void ResetAnimationToFirstFrame();

	/* Gets the y crop based on the current anim state */
	int GetCorrectYCrop();
	
	float TimePerFrame(); /* Returns time to spend per frame	 */
	float AnimDuration(); /* Returns duration of current anim	 */
	int FramesInAnim();   /* Returns frames in current anim		 */
	int FrameWidth();	  /* Returns frame width of current anim */

	/* Pointer to spriteSurface struct with sprite info */
	SpriteInfo* spriteInfo;

};
#endif // Animationable_H

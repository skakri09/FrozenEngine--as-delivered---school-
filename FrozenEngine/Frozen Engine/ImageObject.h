/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   23:06
	filename: 	Frozen Engine\ImageObject.h
	file path:	FrozenEngine\Frozen Engine
	file base:	ImageObject
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	Base class for all surfaces to be used within the engine.
				The imageObject holds a SDL_Surface*, position, velocity,
				render rect and some other vital information needed by
				the RenderEngine to render a surface. 
*********************************************************************/

#ifndef ImageObject_h__
#define ImageObject_h__

// I N C L U D E S

#include <SDL.h>
#include <string>

#include "Vector2D.h"
#include "xmlSettingsHandling.h"
#include "Surfaces.h"	

// C L A S S  D E C L A R A T I O N

class CImageObject
{
public:
	/* Emtpy Ctor */
	//CImageObject();

	/* Ctor used by derived animationObject. Surface will be set in animationObject Ctor */
	CImageObject(float PositionX = 0, float PositionY = 0, int CropX = 0, int CropY = 0);

	/* Ctor used when surface is set in this Ctor */
	CImageObject(std::string surfaceName, float PositionX = 0, float PositionY = 0, 
		int CropX = 0, int CropY = 0);

	virtual ~CImageObject();

public:
	inline void UpdatePosition(float deltaTics){_imagePos += (_velocity * deltaTics);}
	inline void ApplyGravitation(CVector2D& gravity, float deltatics){_velocity += (gravity * deltatics);}

	virtual void UpdateAnimation();

	bool CheckRectCollision(SDL_Rect& rect1);

	bool CheckRectCollision(CImageObject& otherImgObj);

	/* When calling FlagImageForRemoval, removeImage is set to true,	*/
	/* and on next pass in renderEngines renderlist, it will be deleted	*/
	/* from the render loop.											*/
	void FlagImageForRemoval(){removeImage = true;}
	bool RemoveImage(){return removeImage;}

	// S E T T E R S  &  G E T T E R S

	SDL_Surface* getImageSurface() const { return _imageSurface; }
	virtual void setImageSurface(SDL_Surface* val) { _imageSurface = val; }
	virtual void setImageSurface(std::string surfaceName);

	virtual float getPositionY() { return _imagePos.getY(); }
	virtual void setPositionY(float val) { _imagePos.setY(val); }

	virtual float getPositionX() { return _imagePos.getX(); }
	virtual void setPositionX(float val) { _imagePos.setX(val); }

	float getVelocityX() { return _velocity.getX(); }
	void setVelocityX(float val) { _velocity.setX(val); }

	float getVelocityY() { return _velocity.getY(); }
	void setVelocityY(float val) { _velocity.setY(val); }

	int getImageHeight() { return _imageRenderRect.h; }
	void setImageHeight(int val) { _imageRenderRect.h = val; }

	int getImageWidth() { return _imageRenderRect.w; }
	void setImageWidth(int val) { _imageRenderRect.w = val; }

	int getCropX() { return _imageRenderRect.x; }
	void setCropX(int val) { _imageRenderRect.x = val; }

	int getCropY() { return _imageRenderRect.y; }
	void setCropY(int val) {_imageRenderRect.y = val; }

	std::string& GetSurfaceName();

	void SetImageID(int imageIDval){imageID = imageIDval;}
	int GetImageID(){return imageID;}
	// Returns true if the position of this character is not equal to it's position
	// last time this function was called.
	bool IsMovementUpdate();

protected:
	// Our surface
	SDL_Surface* _imageSurface;

	// Name of the surface associated with this obj
	std::string _surfaceName;

	// Current position of our image
	CVector2D _imagePos;

	// Current velocity of our image
	CVector2D _velocity;

	// Velocity of ImageObject last time we called IsMovementUpdate();
	CVector2D previousQueryVel;

	// The rect of our image we are rendering
	SDL_Rect _imageRenderRect;

	bool _isSprite;

	int imageID;
private:
	bool removeImage;

};

#endif // ImageObject_h__

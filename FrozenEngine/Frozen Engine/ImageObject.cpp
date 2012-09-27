#include "ImageObject.h"

// P U B L I C  F U N C T I O N S

CImageObject::CImageObject(std::string surfaceName, float PositionX /*= 0*/, float PositionY /*= 0*/, int CropX /*= 0*/, int CropY /*= 0*/ )
{
	_surfaceName = surfaceName;
	removeImage = false;
	setImageSurface(CSurfaces::Inst()->GetSurface(surfaceName));

	setPositionX(PositionX);
	setPositionY(PositionY);
	_imageRenderRect.x = CropX;
	_imageRenderRect.y = CropY;
	_velocity = 0;
	previousQueryVel = _velocity;

	if (_imageSurface)
	{
		_imageRenderRect.w = _imageSurface->w;
		_imageRenderRect.h = _imageSurface->h;
	}

	_isSprite = false;
}

CImageObject::CImageObject( float PositionX /*= 0*/, float PositionY /*= 0*/, int CropX /*= 0*/, int CropY /*= 0*/ )
{
	removeImage = false;
	setPositionX(PositionX);
	setPositionY(PositionY);
	_imageRenderRect.x = CropX;
	_imageRenderRect.y = CropY;
	_velocity = 0;
	previousQueryVel = _velocity;
	_isSprite = true;
}

CImageObject::~CImageObject()
{
}


void CImageObject::setImageSurface( std::string surfaceName )
{
	setImageSurface(CSurfaces::Inst()->GetSurface(surfaceName));
	int size = sizeof(_imageSurface);
}


void CImageObject::UpdateAnimation()
{
	//Virtual function used to update animation.
	//Required to put both imageobjects and animobjects in renderengine.
}

bool CImageObject::CheckRectCollision( SDL_Rect& otherRect )
{
	if( (otherRect.x > getPositionX() ) && 
		(otherRect.x < (getPositionX() + getImageWidth())) && 
		(otherRect.y > getPositionY() ) && 
		(otherRect.y < (getPositionY() + getImageHeight())))
	{
		return true;
	}
	else return false;
}

bool CImageObject::CheckRectCollision( CImageObject& otherImgObj )
{
	if( (   otherImgObj.getPositionX() > getPositionX()) 
		&& (otherImgObj.getPositionX() < (getPositionX() + getImageWidth())) 
		&& (otherImgObj.getPositionY() > getPositionY())
		&& (otherImgObj.getPositionY() < (getPositionY() + getImageWidth())) )
	{
		return true;
	}
	else return false;
}

std::string& CImageObject::GetSurfaceName()
{
	return _surfaceName;
}

bool CImageObject::IsMovementUpdate()
{
	if(previousQueryVel != _velocity)
	{
		previousQueryVel = _velocity;
		return true;
	}
	return false;
}

#include "Button.h"


// P U B L I C   F U N C T I O N S
// ===============================

CButton::CButton() 
	: log("Button", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	firstQueryOnIn = true;
	mousePos.w = 0;
	mousePos.h = 0;
}

CButton::CButton(std::string surfaceName, float x, float y) 
	: log("Button", CXmlSettings::Inst()->GetLoggerSettings()->logLevel),
	CRenderableNoAnimation(surfaceName),
	CInteractable()
{	
	CRenderableNoAnimation::SetXPosition(x);
	CRenderableNoAnimation::SetYPosition(y);
	_SetBtnRects();
	_SetBtnStateUp();
	firstQueryOnIn = true;
}

CButton::~CButton()
{

}

void CButton::SetButtonSurface( std::string surfaceName, float x, float y )
{
	CRenderableNoAnimation::SetSurface(surfaceName);
	CRenderableNoAnimation::SetXPosition(x);
	CRenderableNoAnimation::SetYPosition(y);
	log << INFO << "Buttonsurface : " << surfaceName << " has been set." << endl;
}

void CButton::MoveButton( float newX, float newY )
{
	CRenderableNoAnimation::SetXPosition(newX);
	CRenderableNoAnimation::SetYPosition(newY);
}

bool CButton::LeftClicked(bool hold)
{
	if(_IsButtonDown(1, hold))
	{
		return true;
	}
	SetMouseOver();
	return false;
}

bool CButton::MiddleClicked(bool hold)
{
	if(_IsButtonDown(2, hold))
	{
		return true;
	}
	SetMouseOver();
	return false;
}

bool CButton::RightClicked(bool hold)
{
	if(_IsButtonDown(3, hold)	)
	{
		return true;
	}
	else
	{
		SetMouseOver();
		return false;
	}
}

bool CButton::DownScrolled()
{
	if(_CheckButton(5))
		return true;
	else
		return false;
}

bool CButton::UpScrolled()
{
	if(_CheckButton(4))
		return true;
	else
		return false;
}

bool CButton::LeftReleased()
{
	//SetMouseOver();
	if(_IsButtonReleased(1))
	{
		return true;
	}
	if(_currBtnState != BUTTON_DOWN)
		SetMouseOver();
	return false;
}

bool CButton::RightReleased()
{
	//	SetMouseOver();
	if(_IsButtonReleased(3))
	{
		return true;
	}
	if(_currBtnState != BUTTON_DOWN)
		SetMouseOver();
	return false;
}

bool CButton::MiddleReleased()
{
	if(_IsButtonReleased(2))
	{
		return true;
	}
	if(_currBtnState != BUTTON_DOWN)
		SetMouseOver();
	return false;
}

// P R I V A T E   F U N C T I O N S
// =================================

void CButton::SetMouseOver()
{
	if(_currBtnState != BUTTON_DOWN)
	{
		if(CInteractable::IsMouseCollision())
		{
			//log << INFO << "mouseover+++++++" << endl;
			_SetBtnStateMouseover();
		}
		else
		{
			//log << INFO << "mouseOut--------" << endl;
			_SetBtnStateUp();
		}
	}
}

void CButton::_SetBtnStateUp()
{
	//log << INFO << "rect UP" << endl;
	_currBtnState = BUTTON_UP;
	CRenderableNoAnimation::SetRect(_btnStateUp);
	CInteractable::SetRect(_btnStateUp);
}

void CButton::_SetBtnStateMouseover()
{
	//log << INFO << "rect MOUSEOVER" << endl;
	_currBtnState = MOUSEOVER;
	CRenderableNoAnimation::SetRect(_btnStateMouseover);
	CInteractable::SetRect(_btnStateMouseover);
}

void CButton::_SetBtnStateDown()
{
	log << INFO << "rect DOWN" << endl;
	_currBtnState = BUTTON_DOWN;
	CRenderableNoAnimation::SetRect(_btnStateDown);
	CInteractable::SetRect(_btnStateDown);
}

void CButton::_SetBtnRects()
{
	_btnStateUp.x = 0;
	_btnStateUp.y = 0;
	_btnStateUp.w = CRenderableNoAnimation::theSurface->clip_rect.w/3;
	_btnStateUp.h = CRenderableNoAnimation::theSurface->clip_rect.h;

	_btnStateMouseover.x = CRenderableNoAnimation::theSurface->clip_rect.w/3;
	_btnStateMouseover.y = 0;
	_btnStateMouseover.w = CRenderableNoAnimation::theSurface->clip_rect.w/3;
	_btnStateMouseover.h = CRenderableNoAnimation::theSurface->clip_rect.h;

	_btnStateDown.x = ( (CRenderableNoAnimation::theSurface->clip_rect.w/3) * 2);
	_btnStateDown.y = 0;
	_btnStateDown.w = CRenderableNoAnimation::theSurface->clip_rect.w/3;
	_btnStateDown.h = CRenderableNoAnimation::theSurface->clip_rect.h;
}

bool CButton::_IsButtonDown( int mouseBtn, bool hold )
{
	if(_CheckButton(mouseBtn, hold))
	{
		_SetBtnStateDown();
		_btnDownTime = SDL_GetTicks() + 100;
		return true;
	}
	else
	{
		//if(_buttonDown)
		if(_currBtnState == BUTTON_DOWN)
		{
			if(SDL_GetTicks() >= _btnDownTime)
			{
				_SetBtnStateUp();
			}
		}
		return false;
	}
}

bool CButton::_CheckButton( int mouseButton, bool hold)
{
	bool returnVal = false;
	if(hold)
	{
		if(CInput::Inst()->MouseStillDown(mouseButton))
		{

			if(CInteractable::IsMouseCollision())
			{
				log << INFO << "Button clicked" << std::endl;
				returnVal = true;
			}
		}else{returnVal = false;}
	}
	else
	{
		if(CInput::Inst()->MouseDown(mouseButton))
		{
			if(CInteractable::IsMouseCollision())
			{
				log << INFO << "Button clicked" << std::endl;
				returnVal = true;
			}
		}else{returnVal = false;}
	}
	return returnVal;
}

bool CButton::_IsButtonReleased( int mouseBtn )
{

	if(CInput::Inst()->MouseUp(mouseBtn))
	{
		if(CInteractable::IsMouseCollision())
		{
			return true;
		}
	}
	else if(CInput::Inst()->MouseStillDown(mouseBtn))
	{
		if(CInteractable::IsMouseCollision())
		{
			_SetBtnStateDown();
			_currBtnState = BUTTON_DOWN;
		}
		else _currBtnState = BUTTON_UP;
	}else _currBtnState = BUTTON_UP;
	return false;
}

bool CButton::MouseIsOver(bool trueOnEveryQuery)
{
	if(_currBtnState == MOUSEOVER)
	{
		if(firstQueryOnIn)
		{
			firstQueryOnIn = trueOnEveryQuery;
			return true;
		}
	}
	else if(_currBtnState != MOUSEOVER)
	{
		firstQueryOnIn = true;
	}

	return false;
}

void CButton::UpdateSurface()
{

}

#include "TTFtext.h"

// P U B L I C   F U N C T I O N S
// ===============================

CTTFText::CTTFText( std::string fontPath, int fontSize, Uint8 red, Uint8 green, Uint8 blue )
	: log("TTFtext", CXmlSettings::Inst()->GetLoggerSettings()->logLevel), CImageObject("no_surface")
{
	_rdyForuse = true;
	_CtorCalls();
	_font = TTF_OpenFont(fontPath.c_str(), fontSize);
	_fontColor.r = red;
	_fontColor.g = green;
	_fontColor.b = blue;
	
	_CtorErrorHandle();
}

CTTFText::CTTFText( std::string fontPath, int fontSize, SDL_Color textColor )
	: log("TTFtext", CXmlSettings::Inst()->GetLoggerSettings()->logLevel), CImageObject("no_surface")
{
	_rdyForuse = true;
	_CtorCalls();
	_font = TTF_OpenFont(fontPath.c_str(), fontSize);
	_fontColor = textColor;
	_CtorErrorHandle();
}

CTTFText::~CTTFText()
{
	TTF_CloseFont(_font);
	SDL_FreeSurface(_message);
	if(_message)
	{
		delete _message;
		_message = 0;
	}
	TTF_Quit();
}



void CTTFText::MoveText(float newX, float newY)
{
	CImageObject::setPositionX(newX);
	CImageObject::setPositionY(newY);
}

bool CTTFText::TextIsClicked( int mouseBtnToCheck, bool hold )
{
	bool returnVal = false;
	if(!hold)
	{
		if(CInput::Inst()->MouseDown(mouseBtnToCheck))
		{
			SDL_Rect mouseRect = {CInput::Inst()->getMousePos().x, CInput::Inst()->getMousePos().y, 0, 0};
			if(CImageObject::CheckRectCollision(mouseRect))
			{
				log << INFO << "Text clicked" << endl;
				returnVal = true;
			}
		}
	}
	else
	{
		if(CInput::Inst()->MouseStillDown(mouseBtnToCheck))
		{
			SDL_Rect mouseRect = {CInput::Inst()->getMousePos().x, CInput::Inst()->getMousePos().y, 0, 0};
			if(CImageObject::CheckRectCollision(mouseRect))
			{
				log << INFO << "Text clicked" << endl;
				returnVal = true;
			}
		}
	}
	return returnVal;
}

// P R I V A T E   F U N C T I O N S
// =================================

void CTTFText::PrepForRender()
{
	if(_rdyForuse)
	{
		CImageObject::setImageSurface(_message);
	}
	else
		log << WARN << "Text not ready for use. Faulty initialization" << endl;
}

void CTTFText::_CtorCalls()
{
	if(!TTF_WasInit())
	{
		if(TTF_Init() < 0)
		{
			log << WARN << "TTF_Init() error: " << TTF_GetError() << endl;
		}
	}
	_font = NULL;
	_message = NULL;
}

void CTTFText::_CtorErrorHandle()
{
	if(_font == NULL)
	{
		log << WARN << "Font was not loaded" << endl;
		_rdyForuse = false;
	}
}




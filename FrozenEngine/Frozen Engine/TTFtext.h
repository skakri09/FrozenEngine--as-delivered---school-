/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   23:13
	filename: 	TTFtext.h
	file path:	FrozenEngine\Frozen Engine
	file base:	TTFtext
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	Unfinished TTFtext class. Once finished, it will use the
				SDL_Surface* from CImageObject that it inherits, to render
				TTF text to the screen. 
*********************************************************************/
#ifndef TTFtext_h__
#define TTFtext_h__

#include <SDL_ttf.h>
#include <string>

#include "Logger.h"
#include "xmlSettingsHandling.h"
#include "ImageObject.h"
#include "RenderEngine.h"
#include "InputManager.h"

using std::endl;

class CTTFText : public CImageObject
{
public:
	CTTFText(std::string fontPath, int fontSize, Uint8 red, Uint8 green, Uint8 blue);
	CTTFText(std::string fontPath, int fontSize, SDL_Color textColor);
	~CTTFText();

	void MoveText(float newX, float newY);

	// Returns true if mouse action is happening
	// on the correct position
	// btnID 1:	Left mouse button
	// btnID 2:	Middle mouse button
	// btnID 3:	Right mouse button
	// btnID 4:	Mouse wheel up
	// btnID 5:	Mouse wheel down 
	bool TextIsClicked(int mouseBtnToCheck, bool hold = false);

private:
	Logger log;
	
	bool _rdyForuse;

	TTF_Font* _font;
	SDL_Surface* _message;
	SDL_Color _fontColor;

	void PrepForRender();
	void _CtorCalls();
	void _CtorErrorHandle();
};

#endif // TTFtext_h__


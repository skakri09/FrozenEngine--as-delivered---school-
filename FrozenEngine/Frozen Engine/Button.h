/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   15:22
	filename: 	Button.h
	file path:	Engine\Frozen Engine
	file base:	Button
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	A simple button class for drawing a button to the screen
				and interact with it. The button should have 3 states,
				up, mouseover and down, and the image should be a sprite
				with these 3 states, in the mentioned order from left
				to right (button up|mouse over|button down). The class
				have bool functions to check if a mouse button has clicked/
				released the button, or if the mouse is colliding with
				the mouse.
*********************************************************************/
#ifndef Button_h__
#define Button_h__

#include <SDL.h>
#include <string>


#include "Timer.h"
#include "Logger.h"
//#include "ImageObject.h"
#include "InputManager.h"
#include "xmlSettingsHandling.h"

#include "Interactable.h"
#include "RenderableNoAnimation.h"

enum eCurrentButtonState{BUTTON_UP, MOUSEOVER, BUTTON_DOWN};

class CButton : public CInteractable, public CRenderableNoAnimation
{
public:
	CButton();
	CButton(std::string surfaceName, float x, float y);
	~CButton();	 

	/* Sets the surface of the button, alternatively also it's posiition */
	void SetButtonSurface(std::string surfaceName, float x = 0, float y  = 0);
	
	/* Sets the position of the button, */
	void MoveButton(float newX, float newY);

	/* Returns true when the mouse is over the button. If param is set to false, */
	/* true is only returned once each time the mouse collides with the button	 */
	bool MouseIsOver(bool trueOnEveryQuery = false);
	bool LeftClicked(bool hold = false);
	bool RightClicked(bool hold = false);
	bool MiddleClicked(bool hold = false);
	bool DownScrolled();
	bool UpScrolled();
	bool LeftReleased();
	bool RightReleased();
	bool MiddleReleased();

	eCurrentButtonState GetBtnState(){return _currBtnState; }

	void UpdateSurface();
private:
	Logger log;	/* Logger object */
	
	void SetMouseOver(); /* Sets button state to mouseover if so is, and button is not pressed */

	/* The duration (millisec) we want button to stay		*/
	/* down after mouse button is released. 0 = instant up	*/
	unsigned int _btnDownTime;				
	
	SDL_Rect _btnStateUp;			/* The clip rect for button state up				*/
	SDL_Rect _btnStateMouseover;	/* The clip rect for button state mouseover			*/
	SDL_Rect _btnStateDown;			/* The clip rect for button state down				*/
	SDL_Rect mousePos;				/* A SDL rect for the mouse position (x, y, 0, 0)	*/

	void _SetBtnStateUp();			/* Changes the clip-rect to show unpressed button	*/
	void _SetBtnStateMouseover();	/* Changes the clip-rect to show mouseovered button */
	void _SetBtnStateDown();		/* Changes the clip-rect to show pressed button		*/

	// Sets the rects for each of the 3 button states (up/mouseover/down).
	// It presumes the surface we're using are organized equal every time:
	// (btnup|mouseover|btndown). If a different way of organizing button
	// surface is needed, another function or overload is required.
	void _SetBtnRects();

	// Holds the current button state (up/mouseover/down)
	eCurrentButtonState _currBtnState;

	// Returns true if the selected mousebutton is pressed. the bool is wheither
	// or not to check for mousStateDown or just mousedown(single click).
	// Uses same button-codes as _CheckButton, and is called by _CheckButton
	bool _IsButtonDown(int mouseBtn, bool hold);
	bool _IsButtonReleased(int mouseBtn);
	// Returns true if mouse action is happening
	// on the correct position
	// btnID 1:	Left mouse button
	// btnID 2:	Middle mouse button
	// btnID 3:	Right mouse button
	// btnID 4:	Mouse wheel up
	// btnID 5:	Mouse wheel down 
	bool _CheckButton(int mouseButton, bool hold = false);

	/* True when it's the first time we ask if mouse is */
	/* over a button, and it is.						*/
	bool firstQueryOnIn;

};







#endif // Button_h__

/********************************************************************
*	Created:	2012/04/19  12:03
*	Filename: 	GUI_Interface.h
*	Author:		Kristian Skarseth
*	
*	purpose:	An interface for all GUI elements, and classes using
				a GUI element. Gives the ability to overload functions
				so that they may be sent as parameters instead of 
				function pointers.
*********************************************************************/
#ifndef GUI_Interface_H
#define GUI_Interface_H

class CGuiInterface
{
public:

	virtual void OnClick() = 0;
	virtual void OnRelease() = 0;

};

#endif // GUI_Interface_H

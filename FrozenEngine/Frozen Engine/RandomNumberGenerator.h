/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   23:05
	filename:	RandomNumberGenerator.h
	file path:	FrozenEngine\Frozen Engine
	file base:	RandomNumberGenerator
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	Simple class to create a random number between a given
				low and high.
*********************************************************************/

#ifndef RandomNumberGenerator_h__
#define RandomNumberGenerator_h__

#include <ctime>
#include <cstdlib>

class CRandomNumberGenerator
{
public:
	CRandomNumberGenerator();
	~CRandomNumberGenerator();

	int MakeRandomNumber(int lowNr, int highNr);
};



#endif // RandomNumberGenerator_h__

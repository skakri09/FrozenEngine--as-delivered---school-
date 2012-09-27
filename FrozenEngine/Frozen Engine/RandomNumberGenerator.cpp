#include "RandomNumberGenerator.h"


CRandomNumberGenerator::CRandomNumberGenerator()
{
	srand(static_cast<unsigned int>(time(0)));
}

CRandomNumberGenerator::~CRandomNumberGenerator()
{

}

int CRandomNumberGenerator::MakeRandomNumber( int lowNr, int highNr )
{
	return lowNr + (rand() % (highNr - lowNr + 1));
}
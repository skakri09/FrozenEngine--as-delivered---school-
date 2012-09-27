////////////////////////////////////////////////////////////////////////////////////////
//
//	File: GCInfo.cpp
//
//	Original author: McGraw-Hill/Osborne / The Art of C++ - ISBN: 0072255129 / 2004
//	Changed by:	     Peder Thorup, Kristian Skarseth / NITH / 2012
//
//	Description:
//				Each element in the GCPtrs gclist is an object of type GCInfo.
//				The GCInfo object contains a refcount and a reference to its allocated memory
//				
//
////////////////////////////////////////////////////////////////////////////////////////

#include "GCInfo.h"

vector<GCInfo> GCInfo::gclist;//gclist init
bool GCInfo::first;

GCInfo::GCInfo(void* mPtr, unsigned size)
{
	refcount = 1;
	memPtr = mPtr;

	if(size != 0)
	{
		isArray = true;
	}
	else
	{
		isArray = false;
	}

	arraySize = size;
}

bool GCInfo::operator==(const GCInfo &obj) const
{
	return (obj.memPtr == this->memPtr);
}
////////////////////////////////////////////////////////////////////////////////////////
//
//	File: GCInfo.h
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

#ifndef GCINFO_H//Inclusion guard
#define GCINFO_H

// I N C L U D E S  /  H E A D E R S
// =================================

#include <vector>
using std::vector;

// C L A S S   D E C L A R A T I O N
// =================================

// This class defines an element that is stored in the garbage collection information list.
class GCInfo 
{
public:
	GCInfo(void* mPtr, unsigned size=0);

	bool operator==(const GCInfo &obj) const;

	void* memPtr;		//Pointer to memory
	unsigned refcount;	//Reference count
	bool isArray;		//True if array
	unsigned arraySize; //Size of array
	
	static vector<GCInfo> gclist;//Vector holding Garbage Collection list.

	static bool first;
};

#endif//GCINFO_H

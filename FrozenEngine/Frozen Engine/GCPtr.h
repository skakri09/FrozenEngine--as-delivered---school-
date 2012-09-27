////////////////////////////////////////////////////////////////////////////////////////
//
//	File: GCPtr.h
//
//	Original author: McGraw-Hill/Osborne / The Art of C++ - ISBN: 0072255129 / 2004
//	Changed by:	     Peder Thorup, Kristian Skarseth / NITH / 2012
//
//	Description:
//				GCPtr implements a pointer type that uses
//				garbage collection to release unused memory.
//				A GCPtr must only be used to point to memory
//				that was dynamically allocated using new.
//				When used to refer to an allocated array,
//				specify the array size.
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef GCPTR_H //Inclusion guard
#define GCPTR_H

// I N C L U D E S  /  H E A D E R S
// =================================

#include <iostream>
#include <vector>

#include "GCInfo.h"
#include "Iter.h"
#include "Logger.h"

using std::vector;
using std::cout;
using std::endl;

// C L A S S   D E C L A R A T I O N
// =================================

template <class T, int size=0> class GCPtr 
{
private:
	// addr points to the allocated memory to which this GCPtr pointer currently points.
	T *addr;

	//isArray is true if this GCPtr points to an allocated array. It is false otherwise.
	bool isArray; // true if pointing to array

	// If this GCPtr is pointing to an allocated array, then arraySize contains its size.
	unsigned arraySize; // size of the array

	// Return an iterator to pointer info in gclist.
	typename std::vector<GCInfo>::iterator findPtrInfo(T *ptr);

	static Logger log; // Logger

	static int OutOfScopeCount;

public:
	typedef Iter<T> GCiterator; // Define an iterator type for GCPtr<T>.

	// Construct both initialized and uninitialized objects.
	GCPtr(T *t=NULL); 

	// Copy constructor.
	GCPtr(const GCPtr &ob);

	~GCPtr(); // Destructor for GCPTr.

	// Overload assignment of pointer to GCPtr.
	T *operator=(T *t);

	// Overload assignment of GCPtr to GCPtr.
	GCPtr &operator=(GCPtr &rv);

	// Return a reference to the object pointed to by this GCPtr.
	T &operator*();

	// Return the address being pointed to.
	T *operator->();

	// Return a reference to the object at the index specified by i.
	T &operator[](int i);

	// Conversion function to T *.
	operator T *();

	// Return an Iter to the start of the allocated memory. 
	Iter<T> begin();

	// Return an Iter to one past the end of an allocated array.
	Iter<T> end();

	// S T A T I C  F U N C T I O N S
	// ==============================

	// Collect garbage. Returns true if at least one object was freed.
	static bool collect();

	// Return the size of gclist for this type  of GCPtr.
	static int gclistSize();

	// A utility function that displays gclist.
	static void showlist();

	// Clear gclist when program exits.
	static void shutdown();

};//end class GCPtr

template <class T, int size> int GCPtr<T, size>::OutOfScopeCount = 0;

//Logger - levels: INFO, WARN, ERRORX, CRITICAL
//Set to ERRORX or higher to disable GCPtr logging
template <class T, int size> Logger GCPtr<T,size>::log("GCPtr", WARN);

template <class T, int size> bool GCPtr<T, size>::collect();
template <class T, int size> int GCPtr<T, size>::gclistSize();
template <class T, int size> void GCPtr<T, size>::showlist();
template <class T, int size> void GCPtr<T, size>::shutdown();

#include "GCPtr.cpp" //Included .cpp. Needed to split template class

#endif //GCPTR_H
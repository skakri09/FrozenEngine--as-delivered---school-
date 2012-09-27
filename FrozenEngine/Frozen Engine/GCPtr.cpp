////////////////////////////////////////////////////////////////////////////////////////
//
//	File: GCPtr.cpp
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

#define debug_mode 1;

#ifndef GCPTR_CPP//Inclusion guard(cpp included at the end of header)
#define GCPTR_CPP

#include "GCPtr.h"

// P U B L I C   F U N C T I O N S
// ===============================

// Construct both initialized and uninitialized objects.
template <class T, int size> GCPtr<T, size>::GCPtr(T *t)
{ 
	if(GCInfo::first)
	{
		atexit(shutdown); // Register shutdown() as an exit function.
	}

	GCInfo::first = false;

	vector<GCInfo>::iterator p;

	p = findPtrInfo(t);

	// If t is already in gclist, then
	// increment its reference count.
	// Otherwise, add it to the list.
	if(p != GCInfo::gclist.end())
	{
		p->refcount++; // increment ref count
	}
	else // Create and store this entry.
	{ 
		GCInfo gcObj(t, size);
		GCInfo::gclist.push_back(gcObj);
	}

	addr = t;
	arraySize = size;

	log << INFO << "New Ptr: [" << t << "] " << "[";
	if(size == 0) 
	{
		isArray = false;
		log << INFO << "VAR] [" << typeid(*t).name() << "]" <<  endl;	
	}
	else
	{
		isArray = true;
		log << INFO << "ARR] [" << typeid(*t).name() << "] size[ " << size << "]" << endl;
	}
}

// Copy constructor.
template <class T, int size> GCPtr<T, size>::GCPtr(const GCPtr &ob)
{
	vector<GCInfo>::iterator p;

	p = findPtrInfo(ob.addr);
	p->refcount++; // increment ref count
	addr = ob.addr;

	arraySize = ob.arraySize;

	log << INFO << "Ptr Copy:[" << addr << "] [";
	if(arraySize == 0)
	{
		isArray = false;
		log << INFO << "VAR]" <<  endl;
	}
	else
	{
		isArray = true;
		log << INFO << "ARR] size[ " << arraySize << "]" << endl;
	}
}

// Destructor for GCPtr.
template <class T, int size> GCPtr<T, size>::~GCPtr()
{
	vector<GCInfo>::iterator p;
	p = findPtrInfo(addr);

	if(p->refcount)
	{
		p->refcount--; // decrement ref count
	}

	log << INFO << "["<< p->memPtr << "] going out of scope." << endl;

#ifdef debug_mode
	//collect(); //Debug - collect every time
#endif

}

//Clear gclist of GCInfo objects no longer pointed to.
template <class T, int size> bool GCPtr<T, size>::collect() 
{
	bool memfreed = false;

	vector<GCInfo>::iterator p;

#ifdef debug_mode
	log << INFO << "=============== Before GC ================" << endl;
	showlist();
#endif // debug_mode

	for(p = GCInfo::gclist.begin(); p != GCInfo::gclist.end(); )
	{
		//Only need to update iterator when erase() is not called.
		if(p->refcount > 0)
		{
			++p; 
		}
		else
		{
			memfreed = true; //report that allocated memory have been freed

			if(p->memPtr)
			{
				if(p->isArray)
				{
					log << WARN << "Deleting [" << p->memPtr << "]" << endl;
					delete[] p->memPtr;
				}
				else
				{
					log << WARN << "Deleting [" << p->memPtr << "]" << endl;
					delete p->memPtr;
				}
			}
			p = GCInfo::gclist.erase(p); //Erase element from gclist

		}//end else
	}//end for

#ifdef debug_mode
	log << INFO << "================ After GC ================" << endl; 
	showlist();
#endif // debug_mode

	return memfreed;
}

// Overload assignment of pointer to GCPtr.
template <class T, int size> T* GCPtr<T, size>::operator=(T *t)
{
	vector<GCInfo>::iterator p;

	// First, decrement the reference count for the memory currently being pointed to.
	p = findPtrInfo(addr);
	p->refcount--;

	//Next, if the new address is already
	//existent in the system, increment its
	//count. Otherwise, create a new entry
	//for gclist.
	p = findPtrInfo(t);

	if(p != GCInfo::gclist.end())
	{
		p->refcount++;
	}
	else // Create and store this entry.
	{
		GCInfo gcObj(t, size);
		GCInfo::gclist.push_back(gcObj);
	}

	addr = t; // store the address.
	return t;
}

// Overload assignment of GCPtr to GCPtr.
template <class T, int size> GCPtr<T, size> & GCPtr<T, size>::operator=(GCPtr &rv)
{
	vector<GCInfo>::iterator p;

	// First, decrement the reference count for the memory currently being pointed to.
	p = findPtrInfo(addr);
	p->refcount--;

	// Next, increment the reference count of the new address.
	p = findPtrInfo(rv.addr);

	p->refcount++; // increment ref count
	addr = rv.addr;// store the address.

	return rv;
}

// Return a reference to the object pointed to by this GCPtr.
template <class T, int size> T& GCPtr<T, size>::operator*()
{
	return *addr;
}

// Return the address being pointed to.
template <class T, int size> T* GCPtr<T, size>::operator->()
{
	return addr;
}

// Return a reference to the object at the index specified by i.
template <class T, int size> T& GCPtr<T, size>::operator[](int i)
{
	return addr[i];
}

// Conversion function to T *.
template <class T, int size> GCPtr<T, size>::operator T *()
{
	return addr;
}

// Return an Iter to the start of the allocated memory. 
template <class T, int size> Iter<T> GCPtr<T, size>::begin()
{
	int size;

	if(isArray)
	{
		size = arraySize;
	}
	else
	{
		size = 1;
	}

	return Iter<T>(addr, addr, addr + size);
}

// Return an Iter to one past the end of an allocated array.
template <class T, int size> Iter<T> GCPtr<T, size>::end()
{
	int size;

	if(isArray)
	{
		size = arraySize;
	}
	else
	{
		size = 1;
	}

	return Iter<T>(addr + size, addr, addr + size);
}

// Return the size of gclist for this type  of GCPtr.
template <class T, int size> int GCPtr<T, size>::gclistSize()
{
	return GCInfo::gclist.size();
}

// Clear gclist when program exits.
template <class T, int size> void GCPtr<T, size>::shutdown() 
{
	if(gclistSize() == 0) // gclist is empty
	{
		return; 
	} 

	vector<GCInfo>::iterator p;

	for(p = GCInfo::gclist.begin(); p != GCInfo::gclist.end(); p++) // Set all reference counts to zero
	{
		p->refcount = 0;
	}

	log << WARN << "Shutting down. Clearing all GCPtr's." << endl;
	log << INFO << "Before shutdown collect." << typeid(T).name() << endl;

	collect();

	log << INFO << "After shutdown collect." << typeid(T).name() << endl;
}

// A utility function that displays gclist.
template <class T, int size> void GCPtr<T, size>::showlist()
{
	vector<GCInfo>::iterator p;

	log << INFO << "Address\t\tRefcount" << endl;

	for(p = GCInfo::gclist.begin(); p != GCInfo::gclist.end(); p++)
	{
		log << INFO << "[" << p->memPtr << "]" << "\t\t" << p->refcount << endl;
	}
}


// P R I V A T E   F U N C T I O N S
// =================================

// Find a pointer in gclist.
template <class T, int size> typename vector<GCInfo>::iterator GCPtr<T, size>::findPtrInfo(T *ptr)
{
	vector<GCInfo>::iterator p;

	for(p = GCInfo::gclist.begin(); p != GCInfo::gclist.end(); p++) //Find ptr in gclist.
	{
		if(p->memPtr == ptr)
		{
			return p;
		}
	}
	return p;
}

#endif//GCPTR_CPP
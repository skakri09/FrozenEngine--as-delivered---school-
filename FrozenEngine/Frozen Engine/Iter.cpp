////////////////////////////////////////////////////////////////////////////////////////
//
//	File: Iter.cpp
//
//	Original author: McGraw-Hill/Osborne / The Art of C++ - ISBN: 0072255129 / 2004
//	Changed by:	     Peder Thorup, Kristian Skarseth / NITH / 2012
//
//	Description:
//				Template class that enables you to cycle through elements of dynamically
//				allocated array.
//				
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef ITER_CPP//Inclusion guard(cpp included at the end of header)
#define ITER_CPP

#include "Iter.h"

// P U B L I C   F U N C T I O N S
// ===============================

//No param constructor
template <class T>Iter<T>::Iter()
{
	ptr = end = begin = NULL;
	length = 0;
}

//Constructor
template <class T>Iter<T>::Iter(T *p, T *first, T *last)
{
	ptr = p;
	end = last;
	begin = first;
	length = last - first;
}

// Return length of sequence to which this Iter points.
template <class T> unsigned Iter<T>::size()
{
	return length;
}

// Return value pointed to by ptr. - Do not allow out-of-bounds access.
template <class T> T& Iter<T>::operator*()
{
	if( (ptr >= end) || (ptr < begin) )
	{
		throw OutOfRangeExc();
	}

	return *ptr;
}

// Return address contained in ptr. - Do not allow out-of-bounds access.
template <class T> T* Iter<T>::operator->()
{
	if( (ptr >= end) || (ptr < begin) )
	{
		throw OutOfRangeExc();
	}

	return ptr;
}

// Prefix ++.
template <class T> Iter<T> Iter<T>::operator++()
{
	ptr++;
	return *this;
}

// Prefix --.
template <class T> Iter<T> Iter<T>::operator--()
{
	ptr--;
	return *this;
}

// Postfix ++.
template <class T> Iter<T> Iter<T>::operator++(int notused)
{
	T *tmp = ptr;
	ptr++;

	return Iter<T>(tmp, begin, end);
}

// Postfix --.
template <class T> Iter<T> Iter<T>::operator--(int notused)
{
	T *tmp = ptr;
	ptr--;

	return Iter<T>(tmp, begin, end);
}

template <class T> T& Iter<T>::operator[] (int i) 
{
	if((i < 0) || (i >= (end - begin)))
	{
		throw OutOfRangeExc();
	}

	return ptr[i];
}

// Define the relational operators.
template <class T> bool Iter<T>::operator==(Iter op2)
{
	return ptr == op2.ptr;
}

template <class T> bool Iter<T>::operator!=(Iter op2)
{
	return ptr != op2.ptr;
}

template <class T> bool Iter<T>::operator<(Iter op2)
{
	return ptr < op2.ptr;
}

template <class T> bool Iter<T>::operator<=(Iter op2)
{
	return ptr <= op2.ptr;
}

template <class T> bool Iter<T>::operator>(Iter op2)
{
	return ptr > op2.ptr;
}

template <class T> bool Iter<T>::operator>=(Iter op2)
{
	return ptr >= op2.ptr;
}

// Subtract an integer from an Iter.
template <class T> Iter<T> Iter<T>::operator-(int n)
{
	ptr -= n;
	return *this;
}

// Add an integer to an Iter.
template <class T> Iter<T> Iter<T>::operator+(int n) 
{
	ptr += n;
	return *this;
}

// Return number of elements between two Iters.
template <class T> int Iter<T>::operator-(Iter<T> &itr2)
{
	return ptr - itr2.ptr;
}

#endif//ITER_CPP
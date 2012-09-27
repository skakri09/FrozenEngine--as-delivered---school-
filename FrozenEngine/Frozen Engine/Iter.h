////////////////////////////////////////////////////////////////////////////////////////
//
//	File: Iter.h
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

#ifndef ITER_H//Inclusion guard
#define ITER_H

// C L A S S   D E C L A R A T I O N
// =================================

template <class T> class Iter
{
	T *ptr;			// current pointer value
	T *end;			// points to element one past end
	T *begin;		// points to start of allocated array
	unsigned length;// length of sequence

public:
	//No param constructor
	Iter();

	//Constructor
	Iter(T *p, T *first, T *last);

	// Return length of sequence to which this Iter points.
	unsigned size();

	// Return value pointed to by ptr. - Do not allow out-of-bounds access.
	T &operator*();

	// Return address contained in ptr. - Do not allow out-of-bounds access.
	T *operator->();

	// Prefix ++.
	Iter operator++();

	// Prefix --.
	Iter operator--();

	// Postfix ++.
	Iter operator++(int notused);

	// Postfix --.
	Iter operator--(int notused);

	T &operator[](int i);

	// Define the relational operators.
	bool operator==(Iter op2);
	bool operator!=(Iter op2);
	bool operator<(Iter op2);
	bool operator<=(Iter op2);
	bool operator>(Iter op2);
	bool operator>=(Iter op2);

	// Subtract an integer from an Iter.
	Iter operator-(int n);

	// Add an integer to an Iter.
	Iter operator+(int n);

	// Return number of elements between two Iters.
	int operator-(Iter<T> &itr2);
};

#include "Iter.h" //.CPP included to split template class for better readability

#endif //ITER_H
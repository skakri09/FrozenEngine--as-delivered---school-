/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   23:02
	filename: 	Vector2D.h
	file path:	FrozenEngine\Frozen Engine
	file base:	Vector2D
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	Simple vector 2d class. Can be used for anything from
				a position in 2d, to acceleration, force, velocity and so on.
*********************************************************************/
#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>

#include "GCPtr.h"
#include "Logger.h"
#include "Timer.h"


class CVector2D
{
public:

	CVector2D(float x = 0, float y = 0);
	~CVector2D();

	inline void setX(float x) {this->x = x;}
	inline void setY(float y) {this->y = y;}
	inline float getX(){return x;}
	inline float getY(){return y;}

	void Invert();
	void InvertX();
	void INvertY();
	// Rotates the vector with an angle
	void Rotate(const float angle);

	// Returns the magnitude of this vector
	float Magnitude() const;

	// Normalizes this vector - Converts it to a unit vector, and returns 
	// previous magnitude
	float Normalize(); 

	// Returns the dot product of this vector and param vector
	float DotProduct(const CVector2D& vector2) const;

	// Returns the cross product of this vector and the param vector
	float CrossProduct(const CVector2D& vector2) const;

	// Get the distance between two vectors
	static float Distance(const CVector2D& vector1, CVector2D& vector2);

	// Give left CVector2D the values of right CVector2D
	CVector2D& operator= (const CVector2D& vector2);

	// Add the values of right CVector2D to the right CVector2D
	CVector2D& operator+=(const CVector2D& vector2);

	// Subtract the values of right CVector2D from left CVector2D
	CVector2D& operator-=(const CVector2D& vector2);

	// Multiply the CVector2D with a scalar value
	CVector2D& operator*=(const float scalar);

	// Divide the CVector2D with a scalar value
	CVector2D& operator/=(const float scalar);

	// Adds left and right CVector2D and returns the value
	const CVector2D operator+(const CVector2D& vector2) const;

	// Subtracts right CVector2D from left CVector2D and returns the value
	const CVector2D operator-(const CVector2D& vector2) const;

	// Multiplies CVector2D with the scalar and returns the value
	const CVector2D operator*(const float scalar) const;

	// Divides CVector2D with the scalar and returns the value
	const CVector2D operator/(const float scalar) const;

	// Returns true if left CVector2D equals the right CVector2D. False otherwise
	bool operator==(const CVector2D& vector2) const;

	// Returns rue if left CVector2D is not equal to the right CVector2D. False otherwise.
	bool operator!=(const CVector2D& vector2) const;


private:
	float x;
	float y;

};

#endif


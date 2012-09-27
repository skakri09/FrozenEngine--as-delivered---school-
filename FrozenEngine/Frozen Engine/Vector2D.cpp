#include "Vector2D.h"

CVector2D::CVector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}
CVector2D::~CVector2D()
{
}

float CVector2D::Distance(const CVector2D& vector1, CVector2D& vector2)
{
	return sqrt( pow((vector2.x - vector1.x), 2) + 
				  pow((vector2.x - vector1.y), 2) );
}

CVector2D& CVector2D::operator= (const CVector2D& vector2)
{
	if(this == &vector2)
		return *this;

	x = vector2.x;
	y = vector2.y;
	return *this;
}

CVector2D& CVector2D::operator+=(const CVector2D& vector2)
{
	x += vector2.x;
	y += vector2.y;
	return *this;
}

CVector2D& CVector2D::operator-=(const CVector2D& vector2)
{
	x -= vector2.x;
	y -= vector2.y;
	return *this;
}

CVector2D& CVector2D::operator*=(const float scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}

CVector2D& CVector2D::operator/=(const float scalar)
{
	x /= scalar;
	y /= scalar;
	return *this;
}

const CVector2D CVector2D::operator+(const CVector2D& vector2) const
{
	return (CVector2D(*this) += vector2);
}

const CVector2D CVector2D::operator-(const CVector2D& vector2) const
{
	return (CVector2D(*this) -= vector2);
}

const CVector2D CVector2D::operator*(const float scalar) const
{
	return (CVector2D(*this) *= scalar);
}

const CVector2D CVector2D::operator/(const float scalar) const
{
	return (CVector2D(*this) /= scalar);
}

bool CVector2D::operator==(const CVector2D& vector2) const
{
	return ((x == vector2.x) && (y == vector2.y));
}

bool CVector2D::operator!=(const CVector2D& vector2) const
{
	return !((x == vector2.x) && (y == vector2.y));
}

void CVector2D::Invert()
{
	x > 0 ? x=(0-x) : x = abs(x);
	y > 0 ? y=(0-x) : y = abs(x);
}

void CVector2D::InvertX()
{
	x > 0 ? x=(0-x) : x = abs(x);
}

void CVector2D::INvertY()
{
	y > 0 ? y=(0-x) : y = abs(x);
}


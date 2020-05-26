#include "pch.h"
#include "vector.h"

float3 float3::Zero			= float3(0.f, 0.f, 0.f);
float3 float3::Right		= float3(1.f, 0.f, 0.f);
float3 float3::Up			= float3(0.f, 1.f, 0.f);
float3 float3::Forward		= float3(0.f, 0.f, 1.f);
float3 float3::NegativeX	= float3(-1.f, 0.f, 0.f);
float3 float3::NegativeY	= float3(0.f, -1.f, 0.f);
float3 float3::NegativeZ;

float3::float3() :
	_x(0.f),
	_y(0.f),
	_z(0.f)
{
}

float3::float3(float x) :
	_x(x),
	_y(0.f),
	_z(0.f)
{
}

float3::float3(float x, float y) :
	_x(x),
	_y(y),
	_z(0.f)
{
}

float3::float3(float x, float y, float z) :
	_x(x),
	_y(y),
	_z(z)
{
}

float3 & float3::operator=(const float3 & rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	this->_x = rhs._x;
	this->_y = rhs._y;
	this->_z = rhs._z;

	return *this;
}

float3 float3::operator+(const float3 & rhs) const
{
	return float3(this->_x + rhs._x, this->_y + rhs._y, this->_z + rhs._z);
}

float3 & float3::operator+=(const float3 & rhs)
{
	this->_x += rhs._x;
	this->_y += rhs._y;
	this->_z += rhs._z;

	return *this;
}

float3 float3::operator-(const float3 & rhs) const
{
	return float3(this->_x - rhs._x, this->_y - rhs._y, this->_z - rhs._z);
}

float3 & float3::operator-=(const float3 & rhs)
{
	this->_x -= rhs._x;
	this->_y -= rhs._y;
	this->_z -= rhs._z;

	return *this;
}

float float3::Dot(const float3 lhs, const float3 rhs)
{
	return (lhs._x * rhs._x) + (lhs._y * rhs._y) + (lhs._z * rhs._z);
}

float3 float3::Cross(const float3 lhs, const float3 rhs)
{
	float x = (lhs._v[1] * rhs._v[2]) - (lhs._v[2] * rhs._v[1]);
	float y = (lhs._v[2] * rhs._v[0]) - (lhs._v[0] * rhs._v[2]);
	float z = (lhs._v[0] * rhs._v[1]) - (lhs._v[1] * rhs._v[0]);

	return float3(x, y, z);
}

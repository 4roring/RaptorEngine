#include "pch.h"
#include "vector.h"

#include "Math.h"

Vector3 Vector3::Zero			= Vector3(0.f, 0.f, 0.f);
Vector3 Vector3::Right			= Vector3(1.f, 0.f, 0.f);
Vector3 Vector3::Up				= Vector3(0.f, 1.f, 0.f);
Vector3 Vector3::Forward		= Vector3(0.f, 0.f, 1.f);
Vector3 Vector3::NegativeX		= Vector3(-1.f, 0.f, 0.f);
Vector3 Vector3::NegativeY		= Vector3(0.f, -1.f, 0.f);
Vector3 Vector3::NegativeZ;

Vector3::Vector3() :
	x(0.f),
	y(0.f),
	z(0.f)
{
}

Vector3::Vector3(float X) :
	x(X),
	y(0.f),
	z(0.f)
{
}

Vector3::Vector3(float X, float Y) :
	x(X),
	y(Y),
	z(0.f)
{
}

Vector3::Vector3(float X, float Y, float Z) :
	x(X),
	y(Y),
	z(Z)
{
}

Vector3::Vector3(const Vector3& rhs) :
	x(rhs.x),
	y(rhs.y),
	z(rhs.z)
{
}

Vector3 & Vector3::operator=(const Vector3 & rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;

	return *this;
}

Vector3 Vector3::operator+(const Vector3 & rhs) const
{
	return Vector3(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
}

Vector3 & Vector3::operator+=(const Vector3 & rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;

	return *this;
}

Vector3 Vector3::operator-(const Vector3 & rhs) const
{
	return Vector3(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
}

Vector3 & Vector3::operator-=(const Vector3 & rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;

	return *this;
}

Vector3 Vector3::operator*(float scale) const
{
	return Vector3(this->x * scale, this->y * scale, this->z * scale);
}

Vector3 & Vector3::operator*=(float scale)
{
	this->x *= scale;
	this->y *= scale;
	this->z *= scale;

	return *this;
}

Vector3 Vector3::operator/(float scale) const
{
	return Vector3(this->x / scale, this->y / scale, this->z / scale);
}

Vector3 & Vector3::operator/=(float scale)
{
	this->x /= scale;
	this->y /= scale;
	this->z /= scale;

	return *this;
}

float Vector3::Length()
{
	return sqrtf((x * x) + (y * y) + (z * z));
}

float Vector3::LengthSq()
{
	return (x * x) + (y * y) + (z * z);
}

bool Vector3::IsNearlyZero(float tolerance) const
{
	return (
		Math::Abs(x) <= tolerance &&
		Math::Abs(y) <= tolerance &&
		Math::Abs(z) <= tolerance );
}

bool Vector3::operator==(const Vector3 & rhs) const
{
	return (x == rhs.x && y == rhs.y && z == rhs.z);
}

float Vector3::Distance(const Vector3 & src, const Vector3 & dst)
{
	return sqrtf(DistanceSq(src, dst));
}

float Vector3::DistanceSq(const Vector3 & src, const Vector3 & dst)
{
	return Math::Square(dst.x - src.x) + Math::Square(dst.y - src.y) + Math::Square(dst.z - src.z);
}

float Vector3::Dot(const Vector3 & lhs, const Vector3 & rhs)
{
	return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

Vector3 Vector3::Cross(const Vector3 & lhs, const Vector3 & rhs)
{
	float x = (lhs.v[1] * rhs.v[2]) - (lhs.v[2] * rhs.v[1]);
	float y = (lhs.v[2] * rhs.v[0]) - (lhs.v[0] * rhs.v[2]);
	float z = (lhs.v[0] * rhs.v[1]) - (lhs.v[1] * rhs.v[0]);

	return Vector3(x, y, z);
}

//=============================================
//					Vector4
//=============================================
Vector4::Vector4() :
	x(0.f),
	y(0.f),
	z(0.f),
	w(0.f)
{
}

Vector4::Vector4(float X) :
	x(X),
	y(0.f),
	z(0.f),
	w(0.f)
{
}

Vector4::Vector4(float X, float Y) :
	x(X),
	y(Y),
	z(0.f),
	w(0.f)
{
}

Vector4::Vector4(float X, float Y, float Z) :
	x(X),
	y(Y),
	z(Z),
	w(0.f)
{
}

Vector4::Vector4(float X, float Y, float Z, float W) :
	x(X),
	y(Y),
	z(Z),
	w(W)
{
}

Vector4::Vector4(const Vector4 & rhs) :
	x(rhs.x),
	y(rhs.y),
	z(rhs.z),
	w(rhs.w)
{
}

Vector4 & Vector4::operator=(const Vector4 & rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;
	this->w = rhs.w;

	return *this;
}

Vector4 Vector4::operator+(const Vector4 & rhs) const
{
	return Vector4(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z, this->w + rhs.w);
}

Vector4 & Vector4::operator+=(const Vector4 & rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
	this->w += rhs.w;

	return *this;
}

Vector4 Vector4::operator-(const Vector4 & rhs) const
{
	return Vector4(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z, this->z - rhs.z);
}

Vector4 & Vector4::operator-=(const Vector4 & rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;
	this->w -= rhs.w;

	return *this;
}

Vector4 Vector4::operator*(float scale) const
{
	return Vector4(this->x * scale, this->y * scale, this->z * scale, this->w * scale);
}

Vector4 & Vector4::operator*=(float scale)
{
	this->x *= scale;
	this->y *= scale;
	this->z *= scale;
	this->w *= scale;

	return *this;
}

Vector4 Vector4::operator/(float scale) const
{
	return Vector4(this->x / scale, this->y / scale, this->z / scale, this->w / scale);
}

Vector4 & Vector4::operator/=(float scale)
{
	this->x /= scale;
	this->y /= scale;
	this->z /= scale;
	this->w /= scale;

	return *this;
}

bool Vector4::IsNearlyZero(float tolerance) const
{
	return (
		Math::Abs(x) <= tolerance &&
		Math::Abs(y) <= tolerance &&
		Math::Abs(z) <= tolerance &&
		Math::Abs(w) <= tolerance );
}

bool Vector4::operator==(const Vector4 & rhs) const
{
	return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
}

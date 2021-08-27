#pragma once

struct Vector3
{
	union
	{
		struct  
		{
			float x, y, z;
		};
		float v[3];
	};

	Vector3();
	Vector3(float X);
	Vector3(float X, float Y);
	Vector3(float X, float Y, float Z);
	Vector3(const Vector3& rhs);
	Vector3& operator=(const Vector3& rhs);

	Vector3 operator+(const Vector3& rhs) const;
	Vector3& operator+=(const Vector3& rhs);

	Vector3 operator-(const Vector3& rhs) const;
	Vector3& operator-=(const Vector3& rhs);

	Vector3 operator*(float scale) const;
	Vector3& operator*=(float scale);

	Vector3 operator/(float scale) const;
	Vector3& operator/=(float scale);

	float Length();
	float LengthSq();

	bool IsNearlyZero(float tolerance) const;

	bool operator==(const Vector3& rhs) const;

	static float Distance(const Vector3& src, const Vector3& dst);
	static float DistanceSq(const Vector3& src, const Vector3& dst);
	static float Dot(const Vector3& lhs, const Vector3& rhs);
	static Vector3 Cross(const Vector3& lhs, const Vector3& rhs);

	static Vector3 Zero;
	static Vector3 Right;
	static Vector3 Up;
	static Vector3 Forward;
	static Vector3 NegativeX;
	static Vector3 NegativeY;
	static Vector3 NegativeZ;
};

struct Vector4
{
	union
	{
		struct
		{
			float x, y, z, w;
		};

		struct 
		{
			float r, g, b, a;
		};
		float v[4];
	};

	Vector4();
	Vector4(float X);
	Vector4(float X, float Y);
	Vector4(float X, float Y, float Z);
	Vector4(float X, float Y, float Z, float W);
	Vector4(const Vector4& rhs);
	Vector4& operator=(const Vector4& rhs);

	Vector4 operator+(const Vector4& rhs) const;
	Vector4& operator+=(const Vector4& rhs);

	Vector4 operator-(const Vector4& rhs) const;
	Vector4& operator-=(const Vector4& rhs);

	Vector4 operator*(float scale) const;
	Vector4& operator*=(float scale);

	Vector4 operator/(float scale) const;
	Vector4& operator/=(float scale);

	bool IsNearlyZero(float tolerance) const;

	bool operator==(const Vector4& rhs) const;

	static Vector4 Zero;
};

typedef Vector4 Color;





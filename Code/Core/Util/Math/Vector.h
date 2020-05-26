#pragma once

#define MYVECTOR
#ifdef MYVECTOR
struct float3
{
	union
	{
		struct  
		{
			float _x, _y, _z;
		};
		float _v[3];
	};

	float3();
	float3(float x);
	float3(float x, float y);
	float3(float x, float y, float z);
	float3& operator=(const float3& rhs);

	float3 operator+(const float3& rhs) const;
	float3& operator+=(const float3& rhs);

	float3 operator-(const float3& rhs) const;
	float3& operator-=(const float3& rhs);

	static float Dot(const float3 lhs, const float3 rhs);
	static float3 Cross(const float3 lhs, const float3 rhs);

	static float3 Zero;
	static float3 Right;
	static float3 Up;
	static float3 Forward;
	static float3 NegativeX;
	static float3 NegativeY;
	static float3 NegativeZ;
};
#else

#include <d3dx10math.h>
typedef D3DXVECTOR3 float3;
typedef D3DXVECTOR4 float4;
typedef D3DXMATRIX float4x4;

#endif




#pragma once

#include <d3dx10math.h>

#ifdef MY_VECTOR
struct float3
{

};
#else
typedef D3DXVECTOR3 float3;
typedef D3DXVECTOR4 float4;
#endif

typedef D3DXMATRIX float4x4;


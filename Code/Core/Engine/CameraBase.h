#pragma once

#include "util/math/Vector.h"

class CameraBase
{
public:
	CameraBase();
	CameraBase(const CameraBase&);
	virtual ~CameraBase();

public:
	virtual bool Init();
	virtual void Release();
	virtual void Render();

public:
	void SetPosition(const float3& position);
	void SetRotation(const float3& rotation);

public:
	const float3&	GetPosition();
	const float3&	GetRotation();
	const float4x4& GetViewMatrix();

private:
	float3 _position;
	float3 _rotation;
	float4x4 _matView;
};

#pragma once

#include "Util/Math/Math.h"

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
	void SetPosition(const Vector3& position);
	void SetRotation(const Vector3& rotation);

public:
	const Vector3&	GetPosition();
	const Vector3&	GetRotation();
	const Matrix4x4& GetViewMatrix();

private:
	Vector3 _position;
	Vector3 _rotation;
	Matrix4x4 _matView;
};

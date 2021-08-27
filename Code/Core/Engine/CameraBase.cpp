#include "pch.h"
#include "CameraBase.h"

CameraBase::CameraBase() :
	_position(0.f, 0.f, 0.f),
	_rotation(0.f, 0.f, 0.f)
{

}

CameraBase::CameraBase(const CameraBase &)
{
}

CameraBase::~CameraBase()
{
}

bool CameraBase::Init()
{
	return false;
}

void CameraBase::Release()
{
}

void CameraBase::Render()
{
	Vector3 up = Vector3(0.f, 1.f, 0.f);
	Vector3 forward = Vector3(0.f, 0.f, 1.f);

	//float yaw = _rotation.x * Math::toDegree;
	//float pitch = _rotation.y * Math::toDegree;
	//float roll = _rotation.z * Math::toDegree;

	Matrix4x4 matRot = Matrix4x4::MakeRotationMatrix(_rotation);

	D3DXVec3TransformCoord(&forward, &forward, &matRot);
	D3DXVec3TransformCoord(&up, &up, &matRot);

	forward = _position + forward;

	D3DXMatrixLookAtLH(&_matView, &_position, &forward, &up);
}

void CameraBase::SetPosition(const Vector3 & position)
{
	_position = position;
}

void CameraBase::SetRotation(const Vector3 & rotation)
{
	_rotation = rotation;
}

const Vector3 & CameraBase::GetPosition()
{
	return _position;
}

const Vector3 & CameraBase::GetRotation()
{
	return _rotation;
}

const Matrix4x4& CameraBase::GetViewMatrix()
{
	return _matView;
}
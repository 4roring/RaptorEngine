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
	float3 up = float3(0.f, 1.f, 0.f);
	float3 forward = float3(0.f, 0.f, 1.f);

	float yaw = _rotation.x * 0.0174532925f;
	float pitch = _rotation.y * 0.0174532925f;
	float roll = _rotation.z * 0.0174532925f;

	float4x4 matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, yaw, pitch, roll);

	D3DXVec3TransformCoord(&forward, &forward, &matRot);
	D3DXVec3TransformCoord(&up, &up, &matRot);

	forward = _position + forward;

	D3DXMatrixLookAtLH(&_matView, &_position, &forward, &up);
}

void CameraBase::SetPosition(const float3 & position)
{
	_position = position;
}

void CameraBase::SetRotation(const float3 & rotation)
{
	_rotation = rotation;
}

const float3 & CameraBase::GetPosition()
{
	return _position;
}

const float3 & CameraBase::GetRotation()
{
	return _rotation;
}

const float4x4 & CameraBase::GetViewMatrix()
{
	return _matView;
}
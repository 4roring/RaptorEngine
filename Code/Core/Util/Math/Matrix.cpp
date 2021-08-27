#include "pch.h"
#include "vector.h"
#include "Matrix.h"

#include "Util/Util.h"

#include <DirectXMath.h>

Matrix4x4::Matrix4x4()
{
	m00 = 0, m01 = 0, m02 = 0, m03 = 0;
	m10 = 0, m11 = 0, m12 = 0, m13 = 0;
	m20 = 0, m21 = 0, m22 = 0, m23 = 0;
	m30 = 0, m31 = 0, m32 = 0, m33 = 0;
}

Matrix4x4::Matrix4x4(const Matrix4x4 & rhs)
{
	m00 = rhs.m00, m01 = rhs.m01, m02 = rhs.m02, m03 = rhs.m03;
	m10 = rhs.m01, m11 = rhs.m11, m12 = rhs.m12, m13 = rhs.m13;
	m20 = rhs.m02, m21 = rhs.m21, m22 = rhs.m22, m23 = rhs.m23;
	m30 = rhs.m03, m31 = rhs.m31, m32 = rhs.m32, m33 = rhs.m33;
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4 & rhs)
{
	m00 = rhs.m00, m01 = rhs.m01, m02 = rhs.m02, m03 = rhs.m03;
	m10 = rhs.m01, m11 = rhs.m11, m12 = rhs.m12, m13 = rhs.m13;
	m20 = rhs.m02, m21 = rhs.m21, m22 = rhs.m22, m23 = rhs.m23;
	m30 = rhs.m03, m31 = rhs.m31, m32 = rhs.m32, m33 = rhs.m33;

	return (*this);
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 & rhs) const
{
	using namespace DirectX;
	Matrix4x4 out;
	XMMATRIX XMatrix1 = XMLoadFloat4x4((const XMFLOAT4X4*)(this));
	XMMATRIX XMatrix2 = XMLoadFloat4x4((const XMFLOAT4X4*)(&rhs));
	XMMATRIX XMatrixResult = XMMatrixMultiply(XMatrix1, XMatrix2);
	XMStoreFloat4x4A( (XMFLOAT4X4A*)(&out), XMatrixResult );

	return out;
}

Matrix4x4 & Matrix4x4::operator*=(const Matrix4x4 & rhs)
{
	using namespace DirectX;
	XMMATRIX XMatrix1 = XMLoadFloat4x4((const XMFLOAT4X4*)(this));
	XMMATRIX XMatrix2 = XMLoadFloat4x4((const XMFLOAT4X4*)(&rhs));
	XMMATRIX XMatrixResult = XMMatrixMultiply(XMatrix1, XMatrix2);
	XMStoreFloat4x4A((XMFLOAT4X4A*)(this), XMatrixResult);

	return *this;
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4 & rhs) const
{
	Matrix4x4 out;
	for (int32 x = 0; x < 4; ++x)
	{
		for (int32 y = 0; y < 4; ++y)
		{
			out.m[x][y] = this->m[x][y] + rhs.m[x][y];
		}
	}

	return out;
}

Matrix4x4 & Matrix4x4::operator+=(const Matrix4x4 & rhs)
{
	return (*this = (*this + rhs));
}

Vector3 Matrix4x4::GetScaledAxis(eAxisType axisType) const
{
	switch (axisType)
	{
	case eAxisType::x:
		return Vector3(m[0][0], m[0][1], m[0][2]);
	case eAxisType::y:
		return Vector3(m[1][0], m[1][1], m[1][2]);
	case eAxisType::z:
		return Vector3(m[2][0], m[2][1], m[2][2]);
	default:
		return Vector3::Zero;
	}
}

float Matrix4x4::Determinant() const
{
	return	m[0][0] * (
		m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
		m[2][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) +
		m[3][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2])
		) -
		m[1][0] * (
			m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
			m[2][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
			m[3][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2])
			) +
		m[2][0] * (
			m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) -
			m[1][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
			m[3][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			) -
		m[3][0] * (
			m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) -
			m[1][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2]) +
			m[2][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			);
}

Matrix4x4 Matrix4x4::Inverse() const
{
	Matrix4x4 out;

	if (GetScaledAxis(eAxisType::x).IsNearlyZero(SMALL_NUMBER) &&
		GetScaledAxis(eAxisType::y).IsNearlyZero(SMALL_NUMBER) &&
		GetScaledAxis(eAxisType::z).IsNearlyZero(SMALL_NUMBER))
	{
		out = Matrix4x4::Identity;
	}
	else
	{
		const float det = Determinant();

		if (det == 0.f)
		{
			out = Matrix4x4::Identity;
		}
		else
		{
			using namespace DirectX;
			XMMATRIX XMSrcMatrix = XMLoadFloat4x4A((const XMFLOAT4X4A*)(this));
			XMMATRIX XMDstMatrix = XMMatrixInverse(nullptr, XMSrcMatrix);
			XMStoreFloat4x4A((XMFLOAT4X4A*)(&out), XMDstMatrix);
		}
	}
	return out;
}

Matrix4x4 Matrix4x4::MakeTranslationMatrix(const Vector3 & position)
{
	Matrix4x4 mat;
	mat.m[0][0] = 1.f;			mat.m[0][1] = 0.f;			mat.m[0][2] = 0.f;			mat.m[0][3] = 0.f;
	mat.m[1][0] = 0.f;			mat.m[1][1] = 1.f;			mat.m[1][2] = 0.f;			mat.m[1][3] = 0.f;
	mat.m[2][0] = 0.f;			mat.m[2][1] = 0.f;			mat.m[2][2] = 1.f;			mat.m[2][3] = 0.f;
	mat.m[3][0] = position.x;	mat.m[3][1] = position.y;	mat.m[3][2] = position.z;	mat.m[3][3] = 1.f;

	return mat;		
}

Matrix4x4 Matrix4x4::MakeRotationMatrix(const Vector3 & eulerAngle)
{
	return MakeRotationMatrixFromZ(eulerAngle.z * Math::toRadian) *
		   MakeRotationMatrixFromX(eulerAngle.x * Math::toRadian) *
		   MakeRotationMatrixFromY(eulerAngle.y * Math::toRadian);
}

Matrix4x4 Matrix4x4::MakeRotationMatrixFromX(float eulerX)
{
	Matrix4x4 mat;
	mat.m[0][0] = 1.f;			mat.m[0][1] = 0.f;			mat.m[0][2] = 0.f;			mat.m[0][3] = 0.f;
	mat.m[1][0] = 0.f;			mat.m[1][1] = cosf(eulerX); mat.m[1][2] = sinf(eulerX);	mat.m[1][3] = 0.f;
	mat.m[2][0] = 0.f;			mat.m[2][1] = -sinf(eulerX);mat.m[2][2] = cosf(eulerX); mat.m[2][3] = 0.f;
	mat.m[3][0] = 0.f;			mat.m[3][1] = 0.f;			mat.m[3][2] = 0.f;			mat.m[3][3] = 1.f;

	return mat;
}

Matrix4x4 Matrix4x4::MakeRotationMatrixFromY(float eulerY)
{
	Matrix4x4 mat;
	mat.m[0][0] = cosf(eulerY); mat.m[0][1] = 0.f;			mat.m[0][2] = -sinf(eulerY);mat.m[0][3] = 0.f;
	mat.m[1][0] = 0.f;			mat.m[1][1] = 1.f;			mat.m[1][2] = 0.f;			mat.m[1][3] = 0.f;
	mat.m[2][0] = sinf(eulerY);	mat.m[2][1] = 0.f;			mat.m[2][2] = cosf(eulerY);	mat.m[2][3] = 0.f;
	mat.m[3][0] = 0.f;			mat.m[3][1] = 0.f;			mat.m[3][2] = 0.f;			mat.m[3][3] = 1.f;

	return mat;
}

Matrix4x4 Matrix4x4::MakeRotationMatrixFromZ(float eulerZ)
{
	Matrix4x4 mat;
	mat.m[0][0] = cosf(eulerZ);	mat.m[0][1] = sinf(eulerZ);	mat.m[0][2] = 0.f;			mat.m[0][3] = 0.f;
	mat.m[1][0] = -sinf(eulerZ);mat.m[1][1] = cosf(eulerZ);	mat.m[1][2] = 0.f;			mat.m[1][3] = 0.f;
	mat.m[2][0] = 0.f;			mat.m[2][1] = 0.f;			mat.m[2][2] = 1.f;			mat.m[2][3] = 0.f;
	mat.m[3][0] = 0.f;			mat.m[3][1] = 0.f;			mat.m[3][2] = 0.f;			mat.m[3][3] = 1.f;

	return mat;
}

Matrix4x4 Matrix4x4::MakeScaleMatrix(float scale)
{
	Matrix4x4 mat;
	mat.m[0][0] = scale;		mat.m[0][1] = 0.f;			mat.m[0][2] = 0.f;			mat.m[0][3] = 0.f;
	mat.m[1][0] = 0.f;			mat.m[1][1] = scale;		mat.m[1][2] = 0.f;			mat.m[1][3] = 0.f;
	mat.m[2][0] = 0.f;			mat.m[2][1] = 0.f;			mat.m[2][2] = scale;		mat.m[2][3] = 0.f;
	mat.m[3][0] = 0.f;			mat.m[3][1] = 0.f;			mat.m[3][2] = 0.f;			mat.m[3][3] = 1.f;

	return mat;
}

Matrix4x4 Matrix4x4::MakeScaleMatrix(const Vector3 & scale)
{
	Matrix4x4 mat;
	mat.m[0][0] = scale.x;		mat.m[0][1] = 0.f;			mat.m[0][2] = 0.f;			mat.m[0][3] = 0.f;
	mat.m[1][0] = 0.f;			mat.m[1][1] = scale.y;		mat.m[1][2] = 0.f;			mat.m[1][3] = 0.f;
	mat.m[2][0] = 0.f;			mat.m[2][1] = 0.f;			mat.m[2][2] = scale.z;		mat.m[2][3] = 0.f;
	mat.m[3][0] = 0.f;			mat.m[3][1] = 0.f;			mat.m[3][2] = 0.f;			mat.m[3][3] = 1.f;

	return mat;
}

Matrix4x4 Matrix4x4::MakeOrthoMatrix()
{
	Matrix4x4 mat;
	mat.m[0][0] = 0.f;			mat.m[0][1] = 0.f;			mat.m[0][2] = 0.f;			mat.m[0][3] = 0.f;
	mat.m[1][0] = 0.f;			mat.m[1][1] = 0.f;			mat.m[1][2] = 0.f;			mat.m[1][3] = 0.f;
	mat.m[2][0] = 0.f;			mat.m[2][1] = 0.f;			mat.m[2][2] = 0.f;			mat.m[2][3] = 0.f;
	mat.m[3][0] = 0.f;			mat.m[3][1] = 0.f;			mat.m[3][2] = 0.f;			mat.m[3][3] = 0.f;

	return mat;
}

Matrix4x4 Matrix4x4::MakePerspectiveMatrix()
{
	Matrix4x4 mat;
	mat.m[0][0] = 0.f;			mat.m[0][1] = 0.f;			mat.m[0][2] = 0.f;			mat.m[0][3] = 0.f;
	mat.m[1][0] = 0.f;			mat.m[1][1] = 0.f;			mat.m[1][2] = 0.f;			mat.m[1][3] = 0.f;
	mat.m[2][0] = 0.f;			mat.m[2][1] = 0.f;			mat.m[2][2] = 0.f;			mat.m[2][3] = 0.f;
	mat.m[3][0] = 0.f;			mat.m[3][1] = 0.f;			mat.m[3][2] = 0.f;			mat.m[3][3] = 0.f;

	return mat;
}

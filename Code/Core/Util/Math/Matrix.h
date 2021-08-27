#pragma once

#include "Util/Util.h"

struct Matrix4x4
{
	union
	{
		struct  
		{
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};
		float m[4][4];
	};

	Matrix4x4();
	Matrix4x4(const Matrix4x4& rhs);
	Matrix4x4& operator=(const Matrix4x4& rhs);

	Matrix4x4 operator*(const Matrix4x4& rhs) const;
	Matrix4x4& operator*=(const Matrix4x4& rhs);

	Matrix4x4 operator+(const Matrix4x4& rhs) const;
	Matrix4x4& operator+=(const Matrix4x4& rhs);

	Vector3 GetScaledAxis(eAxisType axisType) const;

	float Determinant() const;
	Matrix4x4 Inverse() const;

	static Matrix4x4 MakeTranslationMatrix(const Vector3& position);
	// Vector3 
	static Matrix4x4 MakeRotationMatrix(const Vector3& eulerAngle); 
	static Matrix4x4 MakeRotationMatrixFromX(float axisX);
	static Matrix4x4 MakeRotationMatrixFromY(float axisY);
	static Matrix4x4 MakeRotationMatrixFromZ(float axisZ);

	static Matrix4x4 MakeScaleMatrix(float scale);
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	static Matrix4x4 MakeOrthoMatrix();
	static Matrix4x4 MakePerspectiveMatrix();

	static Matrix4x4 Identity;
};






#pragma once

#include "Util/Util.h"
#include "Util/Math/Vector.h"

class Shader
{
private:
	struct MatrixBufferType
	{
		Matrix4x4 matWorld;
		Matrix4x4 matView;
		Matrix4x4 matProj;
	};

public:
	Shader();
	Shader(const Shader&);
	~Shader();

public:
	bool Init();
	void Release();
	bool Render(const Matrix4x4& matWorld, const Matrix4x4& matView, const Matrix4x4& matProj, int32 indexCount);

private:
	bool InitShader(_In_ const TCHAR* shaderFileName);
	void ReleaseShader();
	void OutputShaderErrorMessage(_In_ ID3D10Blob* errorMessage, _In_ const TCHAR* shaderFileName);

	bool SetShaderParameters(Matrix4x4 matWorld, Matrix4x4 matView, Matrix4x4 matProj);
	void RenderShader(int32 indexCount);

private:
	ID3D11VertexShader*		_vertexShader;
	ID3D11PixelShader*		_pixelShader;
	ID3D11InputLayout*		_layout;
	ID3D11Buffer*			_matrixBuffer;
};
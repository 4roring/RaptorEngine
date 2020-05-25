#pragma once

#include <d3d11.h>
#include "Util/Typedef.h"
#include "vector.h"

class Shader
{
private:
	struct MatrixBufferType
	{
		float4x4 matWorld;
		float4x4 matView;
		float4x4 matProj;
	};

public:
	Shader();
	Shader(const Shader&);
	~Shader();

public:
	bool Init();
	void Release();
	bool Render(const float4x4& matWorld, const float4x4& matView, const float4x4& matProj, int32 indexCount);

private:
	bool InitShader(_In_ const TCHAR* shaderFileName);
	void ReleaseShader();
	void OutputShaderErrorMessage(_In_ ID3D10Blob* errorMessage, _In_ const TCHAR* shaderFileName);

	bool SetShaderParameters(float4x4 matWorld, float4x4 matView, float4x4 matProj);
	void RenderShader(int32 indexCount);

private:
	ID3D11VertexShader*		_vertexShader;
	ID3D11PixelShader*		_pixelShader;
	ID3D11InputLayout*		_layout;
	ID3D11Buffer*			_matrixBuffer;
};
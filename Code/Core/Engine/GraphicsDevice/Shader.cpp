#include "pch.h"
#include "Shader.h"

#include <d3d11.h>
#include <D3DX11async.h>
#include "Engine/GraphicsDevice/DeviceDx11.h"
#include <fstream>

Shader::Shader() : 
	_vertexShader(nullptr),
	_pixelShader(nullptr),
	_layout(nullptr),
	_matrixBuffer(nullptr)
{
}

Shader::Shader(const Shader & other)
{
}

Shader::~Shader()
{
}

bool Shader::Init()
{
	bool result = InitShader(TEXT("../../Res/Shader/Color.hlsl"));

	return result;
}

void Shader::Release()
{
	ReleaseShader();
}

bool Shader::Render(const Matrix4x4& matWorld, const Matrix4x4& matView, const Matrix4x4& matProj, int32 indexCount)
{
	bool result = SetShaderParameters(matWorld, matView, matProj);

	if (false == result)
	{
		return false;
	}

	RenderShader(indexCount);

	return true;
}

bool Shader::InitShader(const TCHAR* shaderFileName)
{
	ID3D11Device* device = DeviceDx11::This()->GetDevice();

	HRESULT hResult = E_FAIL;
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob*	pixelShaderBuffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2] = {};
	uint32 numElements = 0;
	D3D11_BUFFER_DESC matrixBufferDesc;

	// VertexShader
	hResult = D3DX11CompileFromFile(
		shaderFileName,
		NULL,
		NULL,
		"ColorVertexShader",
		"vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		NULL,
		&vertexShaderBuffer,
		&errorMessage,
		NULL);

	if (FAILED(hResult))
	{
		if (nullptr != errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, shaderFileName);
		}
		else
		{
			// 메시지 박스 ㄱㄱ
		}
		return false;
	}

	// PixelShader
	hResult = D3DX11CompileFromFile(
		shaderFileName,
		NULL,
		NULL,
		"ColorPixelShader",
		"ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		NULL,
		&pixelShaderBuffer,
		&errorMessage,
		NULL);

	if (FAILED(hResult))
	{
		if (nullptr != errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, shaderFileName);
		}
		else
		{
			// 메시지 박스 ㄱㄱ
		}
		return false;
	}

	hResult = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
	if (FAILED(hResult))
	{
		return false;
	}

	hResult = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
	if (FAILED(hResult))
	{
		return false;
	}

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	hResult = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_layout);
	if (FAILED(hResult))
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	return true;
}

void Shader::ReleaseShader()
{
	if (nullptr != _matrixBuffer)
	{
		_matrixBuffer->Release();
		_matrixBuffer = nullptr;
	}

	if (nullptr != _layout)
	{
		_layout->Release();
		_layout = nullptr;
	}

	if (nullptr != _pixelShader)
	{
		_pixelShader->Release();
		_pixelShader = nullptr;
	}

	if (nullptr != _vertexShader)
	{
		_vertexShader->Release();
		_vertexShader = nullptr;
	}
}


void Shader::OutputShaderErrorMessage(_In_ ID3D10Blob* errorMessage, _In_ const TCHAR* shaderFileName)
{
	char* compileErrors = (char*)(errorMessage->GetBufferPointer());
	ulong bufferSize = errorMessage->GetBufferSize();
	
	std::ofstream fout;
	fout.open("shader_error_log.txt");

	for (ulong i = 0; i < bufferSize; ++i)
	{
		fout << compileErrors[i];
	}

	fout.close();

	errorMessage->Release();
	errorMessage = nullptr;

	// 메시지박스 혹은 어썰트로 컴파일 에러 내용 출력 ㄱㄱ

}

bool Shader::SetShaderParameters(Matrix4x4 matWorld, Matrix4x4 matView, Matrix4x4 matProj)
{
	ID3D11DeviceContext* deviceContext = DeviceDx11::This()->GetDeviceContext();

	HRESULT hResult = E_FAIL;
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	MatrixBufferType* dataPtr;
	uint32 bufferNumber = 0;

	D3DXMatrixTranspose(&matWorld, &matWorld);
	D3DXMatrixTranspose(&matView, &matView);
	D3DXMatrixTranspose(&matProj, &matProj);

	hResult = deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hResult))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->matWorld = matWorld;
	dataPtr->matView = matView;
	dataPtr->matProj = matProj;

	deviceContext->Unmap(_matrixBuffer, 0);

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

	return true;
}

void Shader::RenderShader(int32 indexCount)
{
	ID3D11DeviceContext* deviceContext = DeviceDx11::This()->GetDeviceContext();

	deviceContext->IASetInputLayout(_layout);

	deviceContext->VSSetShader(_vertexShader, NULL, 0);
	deviceContext->PSSetShader(_pixelShader, NULL, 0);

	deviceContext->DrawIndexed(indexCount, 0, 0);
}

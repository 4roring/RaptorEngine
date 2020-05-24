#include "pch.h"
#include "Shape.h"

#include "Engine/GraphicsDevice/DeviceDx11.h"

Shape::Shape() :
	_vertexBuffer(nullptr),
	_indexBuffer(nullptr),
	_vertexCount(0),
	_indexCount(0)
{
}

Shape::Shape(const Shape &)
{
}

Shape::~Shape()
{
}

bool Shape::Init()
{
	bool result = false;

	result = InitializeBuffers();

	return result;
}

void Shape::Release()
{
	ReleseBuffers();
}

void Shape::Render()
{
	RenderBuffers();
}

int32 Shape::GetIndexCount()
{
	return int32();
}

bool Shape::InitializeBuffers()
{
	VertexType* vertices;
	ulong* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT hResult = S_OK;

	_vertexCount = 3;
	_indexCount = 3;

	vertices = new VertexType[_vertexCount];
	if (nullptr == vertices)
	{
		return false;
	}

	indices = new ulong[_indexCount];
	if (nullptr == indices)
	{
		return false;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	ID3D11Device* device = Raptor::DeviceDx11::This()->GetDevice();

	hResult = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(ulong) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	hResult = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return false;
}

void Shape::ReleseBuffers()
{
	if (_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = nullptr;
	}

	if (_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = nullptr;
	}
}

void Shape::RenderBuffers()
{
	ID3D11DeviceContext* deviceContext = Raptor::DeviceDx11::This()->GetDeviceContext();

	uint32 stride;
	uint32 offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

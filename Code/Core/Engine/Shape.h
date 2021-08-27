#pragma once

#include <d3d11.h>
#include "Util/Math/Vector.h"

class Shape
{
private:
	struct VertexType
	{
		Vector3 position;
		Color color;
	};

public:
	Shape();
	Shape(const Shape&);
	virtual ~Shape();

public:
	virtual bool Init();
	virtual void Release();
	virtual void Render();

public:
	int32 GetIndexCount();

private:
	bool InitializeBuffers();
	void ReleseBuffers();
	void RenderBuffers();

private:
	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;
	int32 _vertexCount;
	int32 _indexCount;
};
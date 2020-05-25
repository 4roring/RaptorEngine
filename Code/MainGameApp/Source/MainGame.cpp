#include "pch.h"

#include "MainGame.h"
#include "Engine/GraphicsDevice/DeviceDx11.h"
#include "Engine/CameraBase.h"
#include "Engine/Shape.h"
#include "Engine/GraphicsDevice/Shader.h"

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

void MainGame::Init(bool useVsync, HWND hwnd, bool isFullMode, uint32 width, uint32 height)
{
	bool result = false;

	_graphicDevice = DeviceDx11::This();
	_graphicDevice->InitDevice(useVsync, hwnd, isFullMode, width, height);

	_camera = new CameraBase;
	if (nullptr == _camera)
	{
		return;
	}
	_camera->SetPosition(float3(0.f, 0.f, -10.f));

	_shape = new Shape;
	if (nullptr == _shape)
	{
		return;
	}

	result = _shape->Init();
	if (false == result)
	{
		// error message
		return;
	}

	_shader = new Shader;
	if (nullptr == _shader)
	{
		return;
	}

	result = _shader->Init();
	if (false == result)
	{
		// error message
		return;
	}
}

void MainGame::BeginPlay(bool is_vsync, HWND hwnd, bool is_full_mode, uint32 width, uint32 height)
{
}

void MainGame::GameLoop()
{
	Tick(0.033f);
	Render();
}

void MainGame::Tick(_In_ float deltaTime)
{
}

void MainGame::Render()
{
	float4x4 matWorld, matView, matProj;

	bool result = false;

	_graphicDevice->BeginFrame(0.f, 0.f, 1.f, 1.f);

	_camera->Render();
	matView = _camera->GetViewMatrix();
	
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.f, 1280.f / 800.f, 0.1f, 1000.f);

	_shape->Render();

	_shader->Render(matWorld, matView, matProj, _shape->GetIndexCount());

	_graphicDevice->EndFrame();
}

void MainGame::Release()
{
	if (nullptr != _shader)
	{
		_shader->Release();
		delete _shader;
		_shader = nullptr;
	}

	if (nullptr != _shape)
	{
		_shape->Release();
		delete _shape;
		_shape = nullptr;
	}

	if (nullptr != _camera)
	{
		_camera->Release();
		delete _camera;
		_camera = nullptr;
	}

	_graphicDevice->DestoryDeivce();
	_graphicDevice = nullptr;
}

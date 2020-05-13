#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include "Util/Typedef.h"

namespace Raptor
{

class DeviceDx11
{
private:
	DeviceDx11();
	~DeviceDx11();

public:
	static DeviceDx11* This();

public:
	HRESULT InitDevice(bool isSync, HWND hwnd, bool isFullMode, uint32 width, uint32 height);
	HRESULT DestoryDeivce();

public:
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

public:
	void GetVideocardInfo(_Out_ char* cardName, _Out_ int& memorySize);

public:
	void BeginFrame(float r, float g, float b, float a);
	void EndFrame();

private:
	HRESULT SetupDevice(bool is_vsync, bool is_full_mode);
	HRESULT SetupBackBuffer();
	HRESULT SetupDepthbuffer();
	HRESULT SetupDepthStencilState();
	HRESULT SetupRatserizer();
	HRESULT SetupViewport();
	HRESULT SetupAllMatrix(float z_near, float z_far);

private:
	int _videoCardMemory = 0;
	char _videoCardDesc[128] = {};
	IDXGISwapChain* _swapChain = nullptr;
	ID3D11Device* _device = nullptr;
	ID3D11DeviceContext* _deviceContext = nullptr;
	ID3D11RenderTargetView* _renderTargetView = nullptr;
	ID3D11Texture2D* _depthStencilBuffer = nullptr;
	ID3D11DepthStencilState* _depthStencilState = nullptr;
	ID3D11DepthStencilView* _depthStencilView = nullptr;
	ID3D11RasterizerState* _rasterState = nullptr;
	bool _vsyncEnable = false;
	uint32 _screenWidth = 0;
	uint32 _screenHeight = 0;
	HWND _hwnd = nullptr;
};

}
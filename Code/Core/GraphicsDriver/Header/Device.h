#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include "Typedef.h"

namespace Raptor
{

class Device
{
public:
	Device();
	~Device();

public:
	HRESULT InitDevice(bool is_vsync, HWND hwnd, bool is_full_mode, uint32 width, uint32 height);
	HRESULT DestoryDeivce();

public:
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

public:
	void GetVideocardInfo(char* out_card_name, int& out_memory_size);

public:
	void BeginScene(float r, float g, float b, float a);
	void EndScene();

private:
	HRESULT SetupDevice(bool is_vsync, bool is_full_mode);
	HRESULT SetupBackBuffer();
	HRESULT SetupDepthbuffer();
	HRESULT SetupDepthStencilState();
	HRESULT SetupRatserizer();
	HRESULT SetupViewport();
	HRESULT SetupAllMatrix(float z_near, float z_far);

private:
	int video_card_memory_ = 0;
	char video_card_description_[128] = {};
	IDXGISwapChain* ptr_swap_chain_ = nullptr;
	ID3D11Device* ptr_device_ = nullptr;
	ID3D11DeviceContext* ptr_device_context_ = nullptr;
	ID3D11RenderTargetView* ptr_render_target_view_ = nullptr;
	ID3D11Texture2D* ptr_depth_stencil_buffer_ = nullptr;
	ID3D11DepthStencilState* ptr_depth_stencil_state_ = nullptr;
	ID3D11DepthStencilView* ptr_depth_stencil_view_ = nullptr;
	ID3D11RasterizerState* ptr_raster_state_ = nullptr;
	bool vsync_enable_ = false;
	uint32 width_ = 0;
	uint32 height_ = 0;
	HWND hwnd_ = nullptr;
};

}
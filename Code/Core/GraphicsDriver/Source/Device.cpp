#include "Device.h"

using namespace Raptor;

Device::Device()
{
}

Device::~Device()
{
	
}

HRESULT Device::InitDevice(bool is_vsync, HWND hwnd, bool is_full_mode, uint32 width, uint32 height)
{
	HRESULT result = E_FAIL;

	float fov = 0.f, screen_aspect = 0.f;
	hwnd_ = hwnd;
	width_ = width;
	height_ = height;

	if (FAILED(SetupDevice(is_vsync, is_full_mode)))
		return E_FAIL;

	if (FAILED(SetupBackBuffer()))
		return E_FAIL;

	if (FAILED(SetupDepthbuffer()))
		return E_FAIL;

	if (FAILED(SetupDepthStencilState()))
		return E_FAIL;

	if (FAILED(SetupRatserizer()))
		return E_FAIL;

	if (FAILED(SetupViewport()))
		return E_FAIL;

	//if (FAILED(SetupAllMatrix(z_near, z_far)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT Device::DestoryDeivce()
{
	if (ptr_swap_chain_)
		ptr_swap_chain_->SetFullscreenState(false, nullptr);

	ptr_raster_state_->Release();
	ptr_depth_stencil_view_->Release();
	ptr_depth_stencil_state_->Release();
	ptr_depth_stencil_buffer_->Release();

	ptr_render_target_view_->Release();
	ptr_device_context_->Release();
	ptr_device_->Release();
	ptr_swap_chain_->Release();

	return S_OK;
}

ID3D11Device * Device::GetDevice() const
{
	return ptr_device_;
}

ID3D11DeviceContext * Device::GetDeviceContext() const
{
	return ptr_device_context_;
}

void Device::GetVideocardInfo(char * out_card_name, int & out_memory_size)
{
	strcpy_s(out_card_name, 128, video_card_description_);
	out_memory_size = video_card_memory_;
}

void Device::BeginScene(float r, float g, float b, float a)
{
	const float color[4] = { r, g, b, a };

	ptr_device_context_->ClearRenderTargetView(ptr_render_target_view_, color);
	ptr_device_context_->ClearDepthStencilView(ptr_depth_stencil_view_, D3D11_CLEAR_DEPTH, 1.f, 0);
}

void Device::EndScene()
{
	const unsigned int sync_interval = (true == vsync_enable_) ? 1 : 0;
	ptr_swap_chain_->Present(sync_interval, 0);
}

HRESULT Device::SetupDevice(bool is_vsync, bool is_full_mode)
{
	constexpr int mega_byte = 1024;

	HRESULT result = E_FAIL;
	IDXGIFactory* factory = nullptr;
	IDXGIAdapter* adapter = nullptr;
	IDXGIOutput* adapter_output = nullptr;
	unsigned int num_modes = 0, i = 0, numerator = 0, denominator = 0, str_length = 0;
	DXGI_MODE_DESC* display_mode_list = nullptr;

	DXGI_ADAPTER_DESC adapter_desc = {};
	int error;
	DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
	D3D_FEATURE_LEVEL feature_level = {};

	// vsync Setup
	vsync_enable_ = is_vsync;

	// Create DirectX Graphics Interface Factory
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	//assert(!FAILED(result) && "CreateDXGIFactory Failed");

	// Use the factory to create an adapter for the primary graphics interface(video card)
	result = factory->EnumAdapters(0, &adapter);
	//assert(!FAILED(result) && "Create Adapter Failed");

	// Get the number of modes that fit the Monitor
	result = adapter->EnumOutputs(0, &adapter_output);
	//assert(!FAILED(result) && "Create Adapter Failed");

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output(monitor)
	result = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, NULL);
	//assert(!FAILED(result) && "Display Mode Get Failed");

	// Create a list to hold all the possible display modes for this monitor/video card combination
	display_mode_list = new DXGI_MODE_DESC[num_modes];
	//assert(nullptr != display_mode_list && "Display Mode List Create Failed");

	// Now fill the display mode list structures
	result = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, display_mode_list);
	//assert(!FAILED(result) && "Failed Fill the display mode");

	for (i = 0; i < num_modes; ++i)
	{
		if (display_mode_list[i].Width == (unsigned int)width_ && display_mode_list[i].Height == (unsigned int)height_)
		{
			numerator = display_mode_list[i].RefreshRate.Numerator;
			denominator = display_mode_list[i].RefreshRate.Denominator;
		}
	}

	result = adapter->GetDesc(&adapter_desc);
	//assert(!FAILED(result) && "Failed to Get Adapter Desc");

	video_card_memory_ = (int)(adapter_desc.DedicatedVideoMemory / mega_byte / mega_byte);

	error = wcstombs_s(&str_length, video_card_description_, 128, adapter_desc.Description, 128);
	//assert(0 == error && "Failed to Get Video card Description");

	delete[] display_mode_list;
	display_mode_list = nullptr;

	adapter_output->Release();
	adapter_output = nullptr;

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;

	// Set to a single backbuffer
	swap_chain_desc.BufferCount = 1;

	// Set the width and height of the backbuffer
	swap_chain_desc.BufferDesc.Width = width_;
	swap_chain_desc.BufferDesc.Height = height_;

	// Set regular 32-bit surface for the backbuffer
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	swap_chain_desc.BufferDesc.RefreshRate.Numerator = (vsync_enable_) ? numerator : 0;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = (vsync_enable_) ? denominator : 1;

	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swap_chain_desc.OutputWindow = hwnd_;

	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;

	swap_chain_desc.Windowed = (is_full_mode) ? false : true;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swap_chain_desc.Flags = 0;

	feature_level = D3D_FEATURE_LEVEL_11_0;

	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &feature_level, 1,
		D3D11_SDK_VERSION, &swap_chain_desc, &ptr_swap_chain_, &ptr_device_, nullptr, &ptr_device_context_);
	//assert(!FAILED(result) && "Create to Device And SwapChain Failed");

	return S_OK;
}

HRESULT Device::SetupBackBuffer()
{
	HRESULT result = E_FAIL;
	ID3D11Texture2D* ptr_back_buffer = nullptr;

	result = ptr_swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&ptr_back_buffer);
	//assert(!FAILED(result) && "Failed to Get a BackBuffer Pointer");

	result = ptr_device_->CreateRenderTargetView(ptr_back_buffer, NULL, &ptr_render_target_view_);
	//assert(!FAILED(result) && "Failed to Create RenderTarget");

	ptr_back_buffer->Release();
	ptr_back_buffer = nullptr;

	return S_OK;
}

HRESULT Device::SetupDepthbuffer()
{
	HRESULT result = E_FAIL;
	D3D11_TEXTURE2D_DESC depth_buffer_desc = {};

	depth_buffer_desc.Width = width_;
	depth_buffer_desc.Height = height_;
	depth_buffer_desc.MipLevels = 1;
	depth_buffer_desc.ArraySize = 1;
	depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_buffer_desc.SampleDesc.Count = 1;
	depth_buffer_desc.SampleDesc.Quality = 0;
	depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_buffer_desc.BindFlags = 0;
	depth_buffer_desc.MiscFlags = 0;

	result = ptr_device_->CreateTexture2D(&depth_buffer_desc, NULL, &ptr_depth_stencil_buffer_);
	//assert(!FAILED(result) && "Failed to Create DepthBuffer");
	
	return S_OK;
}

HRESULT Device::SetupDepthStencilState()
{
	HRESULT result = E_FAIL;
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc = {};
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc = {};

	// Setup Depth Stencil State
	depth_stencil_desc.DepthEnable = true;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

	depth_stencil_desc.StencilEnable = true;
	depth_stencil_desc.StencilReadMask = 0xFF;
	depth_stencil_desc.StencilWriteMask = 0xFF;

	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = ptr_device_->CreateDepthStencilState(&depth_stencil_desc, &ptr_depth_stencil_state_);
	//assert(!FAILED(result) && "Failed to Create DepthStencil State");

	ptr_device_context_->OMSetRenderTargets(1, &ptr_render_target_view_, ptr_depth_stencil_view_);

	return S_OK;
}

HRESULT Device::SetupRatserizer()
{
	HRESULT result = E_FAIL;
	D3D11_RASTERIZER_DESC raster_desc = {};

	raster_desc.AntialiasedLineEnable = false;
	raster_desc.CullMode = D3D11_CULL_BACK;
	raster_desc.DepthBias = 0;
	raster_desc.DepthBiasClamp = 0.f;
	raster_desc.DepthClipEnable = true;
	raster_desc.FillMode = D3D11_FILL_SOLID;
	raster_desc.FrontCounterClockwise = false;
	raster_desc.MultisampleEnable = false;
	raster_desc.ScissorEnable = false;
	raster_desc.SlopeScaledDepthBias = 0.f;

	result = ptr_device_->CreateRasterizerState(&raster_desc, &ptr_raster_state_);
	//assert(!FAILED(result) && "Failed to Create Rasterizer State");

	ptr_device_context_->RSSetState(ptr_raster_state_);

	return S_OK;
}

HRESULT Device::SetupViewport()
{
	HRESULT result = E_FAIL;
	D3D11_VIEWPORT viewport = {};

	viewport.Width = (float)width_;
	viewport.Height = (float)height_;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;

	ptr_device_context_->RSSetViewports(1, &viewport);

	return S_OK;
}

HRESULT Device::SetupAllMatrix(float z_near, float z_far)
{
	//const float fov = DirectX::XMConvertToRadians(45.f);
	//const float screen_aspect = (float)kWinCx / (float)kWinCy;

	//projection_matrix = DirectX::XMMatrixPerspectiveFovLH(fov, screen_aspect, z_near, z_far);
	//ortho_matrix = DirectX::XMMatrixOrthographicLH((float)kWinCx, (float)kWinCy, z_near, z_far);
	//view_matrix = DirectX::XMMatrixIdentity();
	//world_matrix = DirectX::XMMatrixIdentity();

	return S_OK;
}

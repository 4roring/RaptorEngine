#include "pch.h"

#include "Engine/GraphicsDevice/DeviceDx11.h"

using namespace Raptor;

DeviceDx11::DeviceDx11()
{
}

DeviceDx11::~DeviceDx11()
{
	
}

DeviceDx11* DeviceDx11::This()
{
	static DeviceDx11 instance;
	return &instance;
}

HRESULT DeviceDx11::InitDevice(bool is_vsync, HWND hwnd, bool is_full_mode, uint32 width, uint32 height)
{
	HRESULT result = E_FAIL;

	float fov = 0.f, screen_aspect = 0.f;
	_hwnd = hwnd;
	_screenWidth = width;
	_screenHeight = height;

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

HRESULT DeviceDx11::DestoryDeivce()
{
	if (_swapChain)
		_swapChain->SetFullscreenState(false, nullptr);

	_rasterState->Release();
	_depthStencilView->Release();
	_depthStencilState->Release();
	_depthStencilBuffer->Release();

	_renderTargetView->Release();
	_deviceContext->Release();
	_device->Release();
	_swapChain->Release();

	return S_OK;
}

ID3D11Device * DeviceDx11::GetDevice() const
{
	return _device;
}

ID3D11DeviceContext * DeviceDx11::GetDeviceContext() const
{
	return _deviceContext;
}

void DeviceDx11::GetVideocardInfo(char * out_card_name, int & memorySize)
{
	strcpy_s(out_card_name, 128, _videoCardDesc);
	memorySize = _videoCardMemory;
}

void DeviceDx11::BeginFrame(float r, float g, float b, float a)
{
	const float color[4] = { r, g, b, a };

	_deviceContext->ClearRenderTargetView(_renderTargetView, color);
	_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);
}

void DeviceDx11::EndFrame()
{
	const unsigned int syncInterval = (true == _vsyncEnable) ? 1 : 0;
	_swapChain->Present(syncInterval, 0);
}

HRESULT DeviceDx11::SetupDevice(bool is_vsync, bool is_full_mode)
{
	constexpr int megaByte = 1024;

	HRESULT result = E_FAIL;
	IDXGIFactory* factory = nullptr;
	IDXGIAdapter* adapter = nullptr;
	IDXGIOutput* adapterOutput = nullptr;
	unsigned int numModes = 0, i = 0, numerator = 0, denominator = 0, strLength = 0;
	DXGI_MODE_DESC* displayModeList = nullptr;

	DXGI_ADAPTER_DESC adapterDesc = {};
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	D3D_FEATURE_LEVEL featureLevel = {};

	// vsync Setup
	_vsyncEnable = is_vsync;

	// Create DirectX Graphics Interface Factory
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	//assert(!FAILED(result) && "CreateDXGIFactory Failed");

	// Use the factory to create an adapter for the primary graphics interface(video card)
	result = factory->EnumAdapters(0, &adapter);
	//assert(!FAILED(result) && "Create Adapter Failed");

	// Get the number of modes that fit the Monitor
	result = adapter->EnumOutputs(0, &adapterOutput);
	//assert(!FAILED(result) && "Create Adapter Failed");

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output(monitor)
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	//assert(!FAILED(result) && "Display Mode Get Failed");

	// Create a list to hold all the possible display modes for this monitor/video card combination
	displayModeList = new DXGI_MODE_DESC[numModes];
	//assert(nullptr != display_mode_list && "Display Mode List Create Failed");

	// Now fill the display mode list structures
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	//assert(!FAILED(result) && "Failed Fill the display mode");

	for (i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width == (unsigned int)_screenWidth && displayModeList[i].Height == (unsigned int)_screenHeight)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	result = adapter->GetDesc(&adapterDesc);
	//assert(!FAILED(result) && "Failed to Get Adapter Desc");

	_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / megaByte / megaByte);

	error = wcstombs_s(&strLength, _videoCardDesc, 128, adapterDesc.Description, 128);
	//assert(0 == error && "Failed to Get Video card Description");

	delete[] displayModeList;
	displayModeList = nullptr;

	adapterOutput->Release();
	adapterOutput = nullptr;

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;

	// Set to a single backbuffer
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the backbuffer
	swapChainDesc.BufferDesc.Width = _screenWidth;
	swapChainDesc.BufferDesc.Height = _screenHeight;

	// Set regular 32-bit surface for the backbuffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	swapChainDesc.BufferDesc.RefreshRate.Numerator = (_vsyncEnable) ? numerator : 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = (_vsyncEnable) ? denominator : 1;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.OutputWindow = _hwnd;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.Windowed = (is_full_mode) ? false : true;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, nullptr, &_deviceContext);
	//assert(!FAILED(result) && "Create to Device And SwapChain Failed");

	return S_OK;
}

HRESULT DeviceDx11::SetupBackBuffer()
{
	HRESULT result = E_FAIL;
	ID3D11Texture2D* ptr_back_buffer = nullptr;

	result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&ptr_back_buffer);
	//assert(!FAILED(result) && "Failed to Get a BackBuffer Pointer");

	result = _device->CreateRenderTargetView(ptr_back_buffer, NULL, &_renderTargetView);
	//assert(!FAILED(result) && "Failed to Create RenderTarget");

	ptr_back_buffer->Release();
	ptr_back_buffer = nullptr;

	return S_OK;
}

HRESULT DeviceDx11::SetupDepthbuffer()
{
	HRESULT result = E_FAIL;
	D3D11_TEXTURE2D_DESC depth_buffer_desc = {};

	depth_buffer_desc.Width = _screenWidth;
	depth_buffer_desc.Height = _screenHeight;
	depth_buffer_desc.MipLevels = 1;
	depth_buffer_desc.ArraySize = 1;
	depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_buffer_desc.SampleDesc.Count = 1;
	depth_buffer_desc.SampleDesc.Quality = 0;
	depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_buffer_desc.BindFlags = 0;
	depth_buffer_desc.MiscFlags = 0;

	result = _device->CreateTexture2D(&depth_buffer_desc, NULL, &_depthStencilBuffer);
	//assert(!FAILED(result) && "Failed to Create DepthBuffer");
	
	return S_OK;
}

HRESULT DeviceDx11::SetupDepthStencilState()
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

	result = _device->CreateDepthStencilState(&depth_stencil_desc, &_depthStencilState);
	//assert(!FAILED(result) && "Failed to Create DepthStencil State");

	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

	return S_OK;
}

HRESULT DeviceDx11::SetupRatserizer()
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

	result = _device->CreateRasterizerState(&raster_desc, &_rasterState);
	//assert(!FAILED(result) && "Failed to Create Rasterizer State");

	_deviceContext->RSSetState(_rasterState);

	return S_OK;
}

HRESULT DeviceDx11::SetupViewport()
{
	HRESULT result = E_FAIL;
	D3D11_VIEWPORT viewport = {};

	viewport.Width = (float)_screenWidth;
	viewport.Height = (float)_screenHeight;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;

	_deviceContext->RSSetViewports(1, &viewport);

	return S_OK;
}

HRESULT DeviceDx11::SetupAllMatrix(float z_near, float z_far)
{
	//const float fov = DirectX::XMConvertToRadians(45.f);
	//const float screen_aspect = (float)kWinCx / (float)kWinCy;

	//projection_matrix = DirectX::XMMatrixPerspectiveFovLH(fov, screen_aspect, z_near, z_far);
	//ortho_matrix = DirectX::XMMatrixOrthographicLH((float)kWinCx, (float)kWinCy, z_near, z_far);
	//view_matrix = DirectX::XMMatrixIdentity();
	//world_matrix = DirectX::XMMatrixIdentity();

	return S_OK;
}

#pragma once

#include "Util/Typedef.h"
//
//namespace Raptor
//{
//	class Device
//	{
//	public:
//		Device();
//		~Device();
//
//	public:
//		HRESULT InitDevice(bool is_vsync, HWND hwnd, bool is_full_mode, uint32 width, uint32 height);
//		HRESULT DestoryDeivce();
//
//	public:
//		ID3D11Device* GetDevice() const;
//		ID3D11DeviceContext* GetDeviceContext() const;
//
//	public:
//		void GetVideocardInfo(char* out_card_name, int& out_memory_size);
//
//	public:
//		void BeginScene(float r, float g, float b, float a);
//		void EndScene();
//
//	private:
//		HRESULT SetupDevice(bool is_vsync, bool is_full_mode);
//		HRESULT SetupBackBuffer();
//		HRESULT SetupDepthbuffer();
//		HRESULT SetupDepthStencilState();
//		HRESULT SetupRatserizer();
//		HRESULT SetupViewport();
//		HRESULT SetupAllMatrix(float z_near, float z_far);
//
//	private:
//		int _videoMemoryCard = 0;
//		char _videoCardDesc[128] = {};
//		bool _vsync_enable = false;
//		uint32 _width = 0;
//		uint32 _height = 0;
//		HWND _hwnd = nullptr;
//	};
//}
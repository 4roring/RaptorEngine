#include "pch.h"

#include "Engine/GraphicsDevice/DeviceBase.h"
#include "DeviceBase.h"

Device::Device()
{
}

Device::~Device()
{
}

bool Device::InitDevice(bool isSync, HWND hwnd, bool isFullMode, uint32 width, uint32 height)
{
	return false;
}

bool Device::DestoryDeivce()
{
	return false;
}

void Device::GetVideocardInfo(char * out_card_name, int & out_memory_size)
{
	__noop;
}

void Device::BeginScene(float r, float g, float b, float a)
{
	__noop;
}

void Device::EndScene()
{
	__noop;
}

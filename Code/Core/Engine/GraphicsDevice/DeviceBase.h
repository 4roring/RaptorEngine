#pragma once

#include "Util/Typedef.h"


class Device
{
public:
	Device();
	virtual ~Device();

public:
	virtual bool InitDevice(bool isSync, HWND hwnd, bool isFullMode, uint32 width, uint32 height);
	virtual bool DestoryDeivce();

public:
	void GetVideocardInfo(char* out_card_name, int& out_memory_size);

public:
	virtual void BeginScene(float r, float g, float b, float a);
	virtual void EndScene();

private:
	int _videoMemoryCard = 0;
	char _videoCardDesc[128] = {};
	bool _vsync_enable = false;
	uint32 _width = 0;
	uint32 _height = 0;
	HWND _hwnd = nullptr;
};

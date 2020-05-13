#pragma once

namespace Raptor
{
	class DeviceDx11;
}

class MainGame
{
public:
	MainGame();
	~MainGame();

public:
	void Init(bool useVsync, HWND hwnd, bool isFullMode, uint32 width, uint32 height);
	void BeginPlay(bool is_vsync, HWND hwnd, bool is_full_mode, uint32 width, uint32 height);
	void GameLoop();

private:
	void Tick(_In_ float deltaTime);
	void Render();

private:
	void Release();
	
private:
	Raptor::DeviceDx11* _graphicDevice = nullptr;
};


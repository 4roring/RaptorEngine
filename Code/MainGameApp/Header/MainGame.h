#pragma once

namespace Raptor
{
	class Device;
}

class MainGame
{
public:
	MainGame();
	~MainGame();

public:
	void BeginPlay(bool is_vsync, HWND hwnd, bool is_full_mode, uint32 width, uint32 height);
	void Run();

private:
	void Tick();
	void Render();

private:
	void Release();
	
private:
	Raptor::Device* ptr_device_ = nullptr;
};


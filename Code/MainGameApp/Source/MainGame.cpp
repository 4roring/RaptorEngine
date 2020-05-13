#include "pch.h"

#include "MainGame.h"
#include "Engine/GraphicsDevice/DeviceDx11.h"

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

void MainGame::Init(bool useVsync, HWND hwnd, bool isFullMode, uint32 width, uint32 height)
{
	_graphicDevice = Raptor::DeviceDx11::This();
	_graphicDevice->InitDevice(useVsync, hwnd, isFullMode, width, height);
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
	_graphicDevice->BeginFrame(0.f, 0.f, 1.f, 0.f);

	_graphicDevice->EndFrame();
}

void MainGame::Release()
{
}

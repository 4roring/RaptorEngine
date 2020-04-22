#include "pch.h"

#include "MainGame.h"
#include "Engine/GraphicsDevice/DeviceDx11.h"

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

void MainGame::BeginPlay(bool is_vsync, HWND hwnd, bool is_full_mode, uint32 width, uint32 height)
{
	ptr_device_ = new Raptor::Device;
	ptr_device_->InitDevice(is_vsync, hwnd, is_full_mode, width, height);
}

void MainGame::Run()
{
	Tick();
	Render();
}

void MainGame::Tick()
{
}

void MainGame::Render()
{
	ptr_device_->BeginScene(0.f, 0.f, 1.f, 0.f);

	ptr_device_->EndScene();
}

void MainGame::Release()
{
}

#pragma once

// ���� ���� �ʱ�ȭ�� ������ ģ����
class DeviceDx11;
class CameraBase;
class Shape;
class Shader;

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

	// ���� ���� �ʱ�ȭ�� ������ ģ����
private:
	DeviceDx11* _graphicDevice = nullptr;
	CameraBase* _camera = nullptr;
	Shape*		_shape = nullptr;
	Shader*		_shader = nullptr;
};


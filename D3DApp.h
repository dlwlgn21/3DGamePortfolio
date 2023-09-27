#pragma once
#include "Common.h"


namespace jh
{

class Camera;
class D3DApp final
{
public:
	static D3DApp& GetInstance()
	{
		static D3DApp instance;
		return instance;
	}

	D3DApp(const D3DApp& other) = delete;
	const D3DApp& operator=(const D3DApp& other) = delete;

	const bool Initialize();
	const int Run();

	void Present();

	HWND GetHwnd() const { assert(mHwnd != nullptr); return mHwnd; }
	UINT GetWidth() const { return mScreenWidth; }
	UINT GetHeight() const { return mScreenHeight; }

	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	const bool IsDrawWire() const { return mbIsDrawWire; }
	const bool IsDrawNormal() const { return mbIsDrawNormal; }

private:
	void update();
	void fixedUpdate();
	void updateGUI();
	void render();
	void onMouseMove(WPARAM btnState, const int mouseX, const int mouseY);

private:
	const bool initializeWindow();
	const bool initGUI();
private:
	D3DApp() = default;
	~D3DApp();

private:
	HWND	mHwnd = NULL;
	HDC		mHdc = NULL;
	UINT	mGuiWidth = 0;
	UINT	mScreenWidth = 1600;
	UINT	mScreenHeight = 900;

	bool	mbIsDrawWire = false;
	bool	mbIsDrawNormal = false;
};

}



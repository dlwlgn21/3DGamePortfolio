#include "D3DApp.h"
#include "GraphicDeviceDX11.h"
#include "GraphicsPSOManager.h"
#include "Time.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Box2DPhysicsManager.h"
#include "Scene.h"

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam);


using namespace jh::graphics;


namespace jh
{
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return D3DApp::GetInstance().MsgProc(hWnd, msg, wParam, lParam);
}


const bool D3DApp::Initialize()
{
	if (!initializeWindow())
	{
		assert(false);
		return false;
	}

	GraphicDeviceDX11::GetInstance().Initialize();
	GraphicsPSOManager::GetInstance().Initialize();
	InputManager::Initialize();
	Time::Initialize();
	Box2DManager::GetInstance().Initialize();
	SceneManager::GetInstance().Initialize();
	if (!initGUI())
	{
		assert(false);
		return false;
	}
	return true;
}

const int D3DApp::Run()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			ImGui_ImplDX11_NewFrame(); // GUI ������ ����
			ImGui_ImplWin32_NewFrame();

			ImGui::NewFrame(); // � �͵��� ������ ���� ��� ����
			ImGui::Begin("Scene Control");

			// ImGui�� �������ִ� Framerate ���
			ImGui::Text(
				"Average %.3f ms/frame (%.1f FPS)",
				1000.0f / ImGui::GetIO().Framerate,
				ImGui::GetIO().Framerate
			);

			updateGUI(); // �߰������� ����� GUI

			mGuiWidth = 0;
			// ȭ���� ũ�� ���� ���� ��� ����
			//ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
			//mGuiWidth = int(ImGui::GetWindowWidth());

			ImGui::End();
			ImGui::Render(); // �������� �͵� ��� ��

			update();
			fixedUpdate();
			render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // GUI ������

			 //Switch the back buffer and the front buffer
			 //����: ImGui RenderDrawData() ������ Present() ȣ��
			Present();
		}
	}

	return 0;
}
void D3DApp::Present()
{
	GraphicDeviceDX11::GetInstance().Present();
}


LRESULT D3DApp::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;

	switch (msg) {
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_MOUSEMOVE:
		// cout << "Mouse " << LOWORD(lParam) << " " << HIWORD(lParam) << endl;
		break;
	case WM_LBUTTONUP:
		// cout << "WM_LBUTTONUP Left mouse button" << endl;
		break;
	case WM_RBUTTONUP:
		// cout << "WM_RBUTTONUP Right mouse button" << endl;
		break;
	case WM_KEYDOWN:
		// cout << "WM_KEYDOWN " << (int)wParam << endl;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

void D3DApp::update()
{
	Time::Update();
	InputManager::Update();
	SceneManager::GetInstance().Update();
}

void D3DApp::fixedUpdate()
{
	SceneManager::GetInstance().FixedUpdate();
	Box2DManager::GetInstance().FixedUpdate();
}

void D3DApp::updateGUI()
{

}

void D3DApp::render()
{
	Time::Render(mHdc);

	auto& gd = GraphicDeviceDX11::GetInstance();
	gd.Clear();
	SceneManager::GetInstance().Render();
	Box2DManager::GetInstance().Render();
}

const bool D3DApp::initializeWindow()
{
	mScreenWidth = 1600;
	mScreenHeight = 900;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	WNDCLASSEX wc = { 
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0L,
		0L,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		L"3DGame",
		NULL 
	};
	if (!RegisterClassEx(&wc)) 
	{
		assert(false);
		return false;
	}
	RECT wr = { 0, 0, static_cast<LONG>(mScreenWidth), static_cast<LONG>(mScreenHeight) };

	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

	mHwnd = CreateWindow(
		wc.lpszClassName, 
		L"PortFolio",
		WS_OVERLAPPEDWINDOW,
		100, // ������ ���� ����� x ��ǥ
		100, // ������ ���� ����� y ��ǥ
		wr.right - wr.left, // ������ ���� ���� �ػ�
		wr.bottom - wr.top, // ������ ���� ���� �ػ�
		NULL, 
		NULL, 
		wc.hInstance, 
		NULL
	);
	if (mHwnd == NULL) 
	{
		assert(false);
		return false;
	}
	mHdc = GetDC(mHwnd);
	assert(mHdc != nullptr);
	ShowWindow(mHwnd, SW_SHOWDEFAULT);
	SetForegroundWindow(mHwnd);
	SetFocus(mHwnd);
	UpdateWindow(mHwnd);
	return true;
}

const bool D3DApp::initGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.DisplaySize = ImVec2(float(mScreenWidth), float(mScreenHeight));
	ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	auto& instance = GraphicDeviceDX11::GetInstance();
	if (!ImGui_ImplDX11_Init(instance.GetDeivceComPtr().Get(), instance.GetDeivceContextComPtr().Get()))
	{
		assert(false);
		return false;
	}

	if (!ImGui_ImplWin32_Init(mHwnd)) 
	{
		assert(false);
		return false;
	}

	return true;
} 

D3DApp::~D3DApp()
{
	SceneManager::GetInstance().Release();
	Box2DManager::GetInstance().Release();
	GraphicsPSOManager::GetInstance().Release();
	//ImGui_ImplDX11_Shutdown();
	//ImGui_ImplWin32_Shutdown();
	//ImGui::DestroyContext();
	GraphicDeviceDX11::GetInstance().Release();
	DestroyWindow(mHwnd);
}

}

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "D3DApp.h"
#include "GraphicDeviceDX11.h"
#include "GraphicsPSOManager.h"
#include "Time.h"
#include "InputManager.h"
#include "SceneManager.h"


#include "LightingManager.h"
#include "CameraManager.h"
#include "CubeMapManager.h"



#include "GameObject.h"
#include "Transform.h"
#include "Light.h"
#include "Scene.h"


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
	SceneManager::GetInstance().Initialize();
	LightingManager::GetInstance().Initialize();
	CubeMapManager::GetInstance().Initialize();
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
			ImGui_ImplDX11_NewFrame(); // GUI 프레임 시작
			ImGui_ImplWin32_NewFrame();

			ImGui::NewFrame(); // 어떤 것들을 렌더링 할지 기록 시작
			ImGui::Begin("Scene Control");

			// ImGui가 측정해주는 Framerate 출력
			ImGui::Text(
				"Average %.3f ms/frame (%.1f FPS)",
				1000.0f / ImGui::GetIO().Framerate,
				ImGui::GetIO().Framerate
			);

			updateGUI(); // 추가적으로 사용할 GUI

			mGuiWidth = 0;
			// 화면을 크게 쓰기 위해 기능 정지
			//ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
			//mGuiWidth = int(ImGui::GetWindowWidth());

			ImGui::End();
			ImGui::Render(); // 렌더링할 것들 기록 끝

			update();
			fixedUpdate();
			render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // GUI 렌더링

			 //Switch the back buffer and the front buffer
			 //주의: ImGui RenderDrawData() 다음에 Present() 호출
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
		onMouseMove(wParam, LOWORD(lParam), HIWORD(lParam));
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
}

void D3DApp::updateGUI()
{
	static const float ANGLE = 180.0f;
	auto& camera = CameraManager::GetInstance().GetCamera();
	auto& dirLight = LightingManager::GetInstance().GetDirectionalLight();
	auto& dirLightPos = dirLight.GetOwner().GetTransform().GetPositionRef();
	ImGui::Checkbox("IsUseDiffuseTexture", LightingManager::GetInstance().GetIsUseDuffuseTexture());
	ImGui::Checkbox("IsUseNormalTexture", LightingManager::GetInstance().GetIsUseNormalTexture());
	ImGui::Checkbox("IsDrawWireFrame", &mbIsDrawWire);
	ImGui::Checkbox("IsDrawNormal", &mbIsDrawNormal);
	//ImGui::SliderFloat("Camera Pitch", &camera.GetTrasform().GetRotation().x, -ANGLE, ANGLE);
	//ImGui::SliderFloat("Camera Roll", &camera.GetRotation().z, -ANGLE, ANGLE);
	//ImGui::SliderFloat("Camera Yaw", &camera.GetRotation().y, -ANGLE, ANGLE);
	ImGui::SliderFloat3("Light Translation", &dirLightPos.x, -10.0f, 10.0f);

}

void D3DApp::render()
{
	Time::Render(mHdc);

	auto& gd = GraphicDeviceDX11::GetInstance();
	gd.Clear();
	SceneManager::GetInstance().Render();
	CubeMapManager::GetInstance().Render();
}

void D3DApp::onMouseMove(WPARAM btnState, const int mouseX, const int mouseY)
{
	float x = mouseX * 2.0f / mScreenWidth - 1.0f;
	float y = -mouseY * 2.0f / mScreenHeight + 1.0f;

	// 커서가 화면 밖으로 나갔을 경우 범위 조절
	// 게임에서는 클램프를 안할 수도 있습니다.
	x = std::clamp(x, -1.0f, 1.0f);
	y = std::clamp(y, -1.0f, 1.0f);

	CameraManager::GetInstance().OnMouseMove(x, y);
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
		100, // 윈도우 좌측 상단의 x 좌표
		100, // 윈도우 좌측 상단의 y 좌표
		wr.right - wr.left, // 윈도우 가로 방향 해상도
		wr.bottom - wr.top, // 윈도우 세로 방향 해상도
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
	GraphicsPSOManager::GetInstance().Release();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	GraphicDeviceDX11::GetInstance().Release();
	DestroyWindow(mHwnd);
}

}


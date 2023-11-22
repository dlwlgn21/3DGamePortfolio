// 2DGamePortfolio.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include <iostream>
#include <memory>
#include <windows.h>
#include "framework.h"
#include "2DGamePortfolio.h"
#include "D3DApp.h"

#include <d3d11.h>
#include <dxgidebug.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

using namespace std;

#ifdef _DEBUG
void D3DMemoryLeakCheck() 
{
    HMODULE dxgiDebugDll = GetModuleHandleW(L"dxgidebug.dll");
    decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(& DXGIGetDebugInterface)>(GetProcAddress(dxgiDebugDll, "DXGIGetDebugInterface"));
    IDXGIDebug* pDebug;
    GetDebugInterface(IID_PPV_ARGS(&pDebug));

    OutputDebugStringW(L"++++++ Direct3D Object ref Count MemoryLeackCheck ++++++\n");
    pDebug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
    OutputDebugStringW(L"++++++ MEMORY LEAK!!! ++++++\n");
    pDebug->Release();
}
#endif // DEBUG


int main() {
    jh::D3DApp& instance = jh::D3DApp::GetInstance();
    if (!instance.Initialize()) 
    {
        assert(false);
        return -1;
    }
    if (instance.Run() == 0)
    {
#ifdef _DEBUG
        D3DMemoryLeakCheck();
#endif // DEBUG
    }
    return 0;
}
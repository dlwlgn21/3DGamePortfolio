#include "GraphicDeviceDX11.h"
#include "D3DApp.h"

using Microsoft::WRL::ComPtr;

namespace jh::graphics
{
	void GraphicDeviceDX11::Initialize()
	{
		ZeroMemory(&mViewPort, sizeof(D3D11_VIEWPORT));
		/// <summary>
		/// 1. Device 와 SwapChain 생성한다.
		/// 2. 백버퍼에 실제로 렌더링할 렌더타겟 뷰를 생성해야한다.
		/// 3. 화면을 클리어 해줘야한다. 뷰포트를 생성해줘야 한다.
		/// 4. 매프레임마다 위에서 생성한 렌더타겟뷰에 렌더링해주어야한다.
		/// 5. Swapchain을 이용하여 최종 디바이스(디스플레이)에 화면을 그려줘야한다.
		/// </summary>
		/// <param name="validationMode"></param>

		HWND hwnd = jh::D3DApp::GetInstance().GetHwnd();

		// Device, Device Context
		UINT DeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL FeatureLevel = static_cast<D3D_FEATURE_LEVEL>(0);

		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			DeviceFlag,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			mcpDevice.GetAddressOf(),
			&FeatureLevel,
			mcpContext.GetAddressOf()
		);

		if (FAILED(hr))
		{
			assert(false);
		}

		// SwapChain
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferDesc.Width = D3DApp::GetInstance().GetWidth();
		swapChainDesc.BufferDesc.Height = D3DApp::GetInstance().GetHeight();
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		if (!createSwapChain(&swapChainDesc))
		{
			assert(false);
			return;
		}

		hr = mcpSwapChain->GetBuffer(0, IID_PPV_ARGS(mcpRenderTargetTextrue.ReleaseAndGetAddressOf()));

		if (FAILED(hr))
		{
			assert(false);
			return;
		}
		hr = mcpDevice->CreateRenderTargetView(
			mcpRenderTargetTextrue.Get(),
			nullptr,
			mcpRenderTargetView.ReleaseAndGetAddressOf()
		);

		if (FAILED(hr))
		{
			assert(false);
			return;
		}

		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
		depthBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		depthBufferDesc.CPUAccessFlags = 0;

		depthBufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.Width = D3DApp::GetInstance().GetWidth();
		depthBufferDesc.Height = D3DApp::GetInstance().GetHeight();
		depthBufferDesc.ArraySize = 1;

		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;

		depthBufferDesc.MiscFlags = 0;
		depthBufferDesc.MipLevels = 0;


		hr = mcpDevice->CreateTexture2D(
			&depthBufferDesc,
			nullptr,
			mcpDepthStencilTextrue.ReleaseAndGetAddressOf()
		);
		if (FAILED(hr))
		{
			assert(false);
		}
		hr = mcpDevice->CreateDepthStencilView(
			mcpDepthStencilTextrue.Get(),
			nullptr,
			mcpDepthStencilView.ReleaseAndGetAddressOf()
		);
		if (FAILED(hr))
		{
			assert(false);
		}

		RECT winRect;
		GetClientRect(D3DApp::GetInstance().GetHwnd(), &winRect);
		/*
		typedef struct D3D11_VIEWPORT
		{
			FLOAT TopLeftX;
			FLOAT TopLeftY;
			FLOAT Width;
			FLOAT Height;
			FLOAT MinDepth;
			FLOAT MaxDepth;
		} 	D3D11_VIEWPORT;
		*/
		mViewPort = 
		{
			0.0f,
			0.0f,
			FLOAT(winRect.right - winRect.left),
			FLOAT(winRect.bottom - winRect.top),
			0.0f,
			1.0f
		};

		mcpContext->RSSetViewports(1, &mViewPort);
		mcpContext->OMSetRenderTargets(
			1,
			mcpRenderTargetView.GetAddressOf(),
			mcpDepthStencilView.Get()
		);
	}
	void GraphicDeviceDX11::Release()
	{
		mcpDepthStencilView.Reset();
		mcpDepthStencilTextrue.Reset();
		mcpRenderTargetView.Reset();
		mcpRenderTargetTextrue.Reset();
		mcpSwapChain.Reset();
		mcpContext.Reset();
		mcpDevice.Reset();
	}

	bool GraphicDeviceDX11::createSwapChain(DXGI_SWAP_CHAIN_DESC* pDesc)
	{
		Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> pDXGIAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> pDXGIFactory = nullptr;

		if (FAILED(mcpDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
		{
			return false;
		}

		if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pDXGIAdapter.GetAddressOf())))
		{
			return false;
		}


		if (FAILED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pDXGIFactory.GetAddressOf())))
		{
			return false;
		}

		if (FAILED(pDXGIFactory->CreateSwapChain(mcpDevice.Get(), pDesc, mcpSwapChain.GetAddressOf())))
		{
			return false;
		}
		return true;
	}
}
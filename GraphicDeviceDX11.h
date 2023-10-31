#pragma once
#include "GraphicsCommon.h"

namespace jh::graphics
{

class GraphicDeviceDX11 final
{
public:
	static GraphicDeviceDX11& GetInstance()
	{
		static GraphicDeviceDX11 instance;
		return instance;
	}

	GraphicDeviceDX11(const GraphicDeviceDX11& other) = delete;
	const GraphicDeviceDX11& operator=(const GraphicDeviceDX11& other) = delete;

	__forceinline void Present();
	__forceinline void SetMainRenderTarget();
	__forceinline void Clear();

	void Initialize();
	void Release();



#pragma region GETTER
	ID3D11Device& GetDeivce() { assert(mcpDevice != nullptr); return *mcpDevice.Get(); }
	ID3D11DeviceContext& GetDeivceContext() { assert(mcpContext != nullptr);  return *mcpContext.Get(); }
	Microsoft::WRL::ComPtr<ID3D11Device>& GetDeivceComPtr() { assert(mcpDevice != nullptr); return mcpDevice; }
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetDeivceContextComPtr() { assert(mcpContext != nullptr);  return mcpContext; }
			
	D3D11_VIEWPORT* GetViewPort() { return &mViewPort; }
	Microsoft::WRL::ComPtr<ID3D11Texture2D>& GetRenderTarget() { return mcpRenderTargetTextrue; }
	Microsoft::WRL::ComPtr<ID3D11Texture2D>& GetDepthStencilBuffer() { return mcpDepthStencilTextrue; }
#pragma endregion
private:
	bool createSwapChain(DXGI_SWAP_CHAIN_DESC* pDesc);


private:
	GraphicDeviceDX11()
		: mcpDevice(nullptr)
		, mcpContext(nullptr)
		, mcpSwapChain(nullptr)
		, mcpRenderTargetTextrue(nullptr)
		, mcpRenderTargetView(nullptr)
		, mcpDepthStencilTextrue(nullptr)
		, mcpDepthStencilView(nullptr)
		, mViewPort()
	{
	}
	~GraphicDeviceDX11() = default;

private:
	Microsoft::WRL::ComPtr<ID3D11Device>			mcpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		mcpContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			mcpSwapChain;

	Microsoft::WRL::ComPtr<ID3D11Texture2D>			mcpRenderTargetTextrue;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  mcpRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			mcpDepthStencilTextrue;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	mcpDepthStencilView;
	D3D11_VIEWPORT									mViewPort;
};

void GraphicDeviceDX11::Present()
{
	mcpSwapChain->Present(1, 0);
}

void GraphicDeviceDX11::Clear()
{
	const static FLOAT BACKGROUND_COLOR[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	mcpContext->ClearRenderTargetView(mcpRenderTargetView.Get(), BACKGROUND_COLOR);
	mcpContext->ClearDepthStencilView(mcpDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicDeviceDX11::SetMainRenderTarget()
{
	mcpContext->OMSetRenderTargets(
		1,
		mcpRenderTargetView.GetAddressOf(),
		mcpDepthStencilView.Get()
	);
}
}
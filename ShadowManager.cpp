#include "ShadowManager.h"
#include "GraphicDeviceDX11.h"
#include "GraphicsPSOManager.h"
#include "SceneManager.h"
#include "PlayScene.h"
#include "GameObject.h"
#include "Renderer.h"


using namespace jh::graphics;
using namespace jh::enums;
namespace jh
{
void ShadowManager::Initialize()
{
    auto& device = GraphicDeviceDX11::GetInstance().GetDeivce();
    // DepthStencilView 만들기
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = mShadowMapping.ShadowViewportWidth;
    desc.Height = mShadowMapping.ShadowViewportHeight;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Format = DXGI_FORMAT_R32_TYPELESS;
    HRESULT hr = device.CreateTexture2D(
        &desc,
        NULL,
        mShadowMapping.mcpShadowBuffer.ReleaseAndGetAddressOf()
    );
    if (FAILED(hr)) { assert(false); }

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(dsvDesc));
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    hr = device.CreateDepthStencilView(
        mShadowMapping.mcpShadowBuffer.Get(),
        &dsvDesc,
        mShadowMapping.mcpShadowDSV.ReleaseAndGetAddressOf()
    );
    if (FAILED(hr)) { assert(false); }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    hr = device.CreateShaderResourceView(
        mShadowMapping.mcpShadowBuffer.Get(),
        &srvDesc,
        mShadowMapping.mcpShadowSRV.ReleaseAndGetAddressOf()
    );
    if (FAILED(hr)) { assert(false); }

    D3D11_VIEWPORT shadowViewport;
    ZeroMemory(&shadowViewport, sizeof(D3D11_VIEWPORT));
    shadowViewport.TopLeftX = 0;
    shadowViewport.TopLeftY = 0;
    shadowViewport.Width = float(mShadowMapping.ShadowViewportWidth);
    shadowViewport.Height = float(mShadowMapping.ShadowViewportHeight);
    shadowViewport.MinDepth = 0.0f;
    shadowViewport.MaxDepth = 1.0f;
    mShadowMapping.Viewport = shadowViewport;
}


void ShadowManager::SetShaowPSO()
{
    auto& dc = GraphicDeviceDX11::GetInstance().GetDeivceContext();
    auto& pso = GraphicsPSOManager::GetInstance().mDepthOnlyPSO;
    dc.OMSetRenderTargets(0, NULL, mShadowMapping.mcpShadowDSV.Get()); // 쉐도우 DSV에다가 출력하고양.
    dc.ClearDepthStencilView(mShadowMapping.mcpShadowDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0); // 클리어 해주고!
    dc.RSSetViewports(1, &mShadowMapping.Viewport);

    dc.IASetInputLayout(pso.mcpInputLayout.Get());
    dc.IASetPrimitiveTopology(pso.mPrimitiveTopology);
    dc.PSSetSamplers(1, 1, pso.mcpSampler.GetAddressOf());
    dc.VSSetShader(pso.mcpVertexShader.Get(), nullptr, 0);
    dc.PSSetShader(pso.mcpPixelShader.Get(), nullptr, 0);
}

void ShadowManager::RenderAtShadowMap()
{
    auto& sceneManager = SceneManager::GetInstance();
    auto& scene = sceneManager.GetCurrentScene();
    auto& layers = scene.GetAllLayers();
   
    for (int i = 0; i < static_cast<UINT>(eLayerType::COUNT); ++i)
    {
        if (layers[i] != nullptr)
        {
            auto& gameobjects = layers[i]->GetAllGameObjects();
            for (auto& gameobject : gameobjects)
            {
               Renderer* pRenderer = static_cast<Renderer*>(gameobject->GetComponentOrNull(eComponentType::RENDERER));
               if (pRenderer != nullptr)
               {
                   pRenderer->ShadowRender();
               }
            }
        }
    }
}

void ShadowManager::SetShadowSRV()
{
    auto& dc = GraphicDeviceDX11::GetInstance().GetDeivceContext();
    dc.PSSetShaderResources(10, 1, mShadowMapping.mcpShadowSRV.GetAddressOf());
}

void ShadowManager::ClearShadowSRV()
{
    auto& dc = GraphicDeviceDX11::GetInstance().GetDeivceContext();
    ID3D11ShaderResourceView* pSrvNullPtr = nullptr;
    dc.PSSetShaderResources(10, 1, &pSrvNullPtr);
}
}
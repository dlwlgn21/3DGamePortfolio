#pragma once
#include "GraphicsCommon.h"

namespace jh
{

struct ShadowMapping
{
    UINT ShadowViewportWidth = 4096;
    UINT ShadowViewportHeight = 4096;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> mcpShadowBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mcpShadowDSV;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mcpShadowSRV;
    D3D11_VIEWPORT Viewport;
};

class ShadowManager final
{
public:
    static ShadowManager& GetInstance()
    {
        static ShadowManager instance;
        return instance;
    }

    ShadowManager(const ShadowManager& other) = delete;
    ShadowManager& operator=(const ShadowManager& other) = delete;


    void Initialize();
    void SetShadowSRV();
    void ClearShadowSRV();
    void RenderAtShadowMap();
    ShadowMapping& GetShadowMapping() { return mShadowMapping; }

private:
    void setShadowPSO();
    void setShadowSkinnedPSO();
private:
    ShadowManager() = default;
    ~ShadowManager()
    {
        mShadowMapping.mcpShadowSRV.Reset();
        mShadowMapping.mcpShadowDSV.Reset();
        mShadowMapping.mcpShadowBuffer.Reset();
    }
    ShadowMapping       mShadowMapping;
};

}


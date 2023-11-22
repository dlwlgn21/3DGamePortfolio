#pragma once
#include "GraphicsCommon.h"

namespace jh::graphics
{
class GraphicsPSO 
{
public:
    ~GraphicsPSO();
    void operator=(const GraphicsPSO& pso);
public:
    Microsoft::WRL::ComPtr<ID3D11VertexShader>      mcpVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>       mcpPixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>       mcpInputLayout;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>   mcpRS;
    Microsoft::WRL::ComPtr<ID3D11SamplerState>      mcpSampler;

    D3D11_PRIMITIVE_TOPOLOGY mPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};
}

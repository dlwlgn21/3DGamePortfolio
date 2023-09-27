#pragma once
#include "Common.h"
#include "Mesh.h"


namespace jh::graphics
{

struct CubeMapping
{
	Mesh* pMesh = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cpCubeMapRSV = nullptr;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> cpVertexShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> cpPixelShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> cpInputLayout = nullptr;

    CubeMapping() = default;
    ~CubeMapping()
    {
        cpInputLayout.Reset();
        cpPixelShader.Reset();
        cpVertexShader.Reset();
        cpCubeMapRSV.Reset();
    }
};

}


#pragma once
#include "Common.h"
#include "Mesh.h"


namespace jh::graphics
{

struct CubeMapping
{
	Mesh* pMesh;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cpCubeMapRSV;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> cpVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> cpPixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> cpInputLayout;

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


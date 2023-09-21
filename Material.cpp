#include "Material.h"
#include "Texture.h"
#include "GraphicDeviceDX11.h"

using namespace jh::enums;

namespace jh::graphics
{

Material::Material()
	: GraphicResource(eResourceType::MATERIAL)
{
	for (UINT i = 0; i < static_cast<UINT>(eTextureType::COUNT); ++i)
	{
		mpTextures[i] = nullptr;
	}
}

void Material::ClearPipeline()
{
	for (UINT i = 0; i < static_cast<UINT>(eTextureType::COUNT); ++i)
	{
		if (mpTextures[i] != nullptr)
		{
			mpTextures[i]->Clear(static_cast<UINT>(i));
		}
	}
	
}

void Material::SetTexture(const eTextureType eType, Texture* pTexture)
{
	assert(mpTextures[static_cast<UINT>(eType)] == nullptr);
	mpTextures[static_cast<UINT>(eType)] = pTexture;
}

void Material::SetPipeline()
{
	assert(mGraphicsPSO.mcpInputLayout != nullptr);
	auto& gdc = GraphicDeviceDX11::GetInstance().GetDeivceContext();

	gdc.IASetInputLayout(mGraphicsPSO.mcpInputLayout.Get());
	gdc.IASetPrimitiveTopology(mGraphicsPSO.mPrimitiveTopology);
	gdc.RSSetState(mGraphicsPSO.mcpRS.Get());
	gdc.VSSetShader(mGraphicsPSO.mcpVertexShader.Get(), nullptr, 0);
	gdc.PSSetShader(mGraphicsPSO.mcpPixelShader.Get(), nullptr, 0);
	//gdc.PSSetSamplers(0, 1, mGraphicsPSO->mcpSampler.GetAddressOf());
		
	for (UINT i = 0; i < static_cast<UINT>(eTextureType::COUNT); ++i)
	{
		if (mpTextures[i] != nullptr)
		{
			mpTextures[i]->PSSetSRV();
		}
	}
}

}
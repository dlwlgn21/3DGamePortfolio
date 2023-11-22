#include "Material.h"
#include "Texture.h"
#include "GraphicDeviceDX11.h"
#include "GraphicsPSOManager.h"
#include "LightingManager.h"
#include "D3DApp.h"

using namespace jh::enums;

namespace jh::graphics
{

Material::Material()
	: GraphicResource(eResourceType::MATERIAL)
	, mpGraphicsPSO(nullptr)
{
}


void Material::SetPipeline()
{
	assert(mpGraphicsPSO->mcpInputLayout != nullptr);
	auto& gdc = GraphicDeviceDX11::GetInstance().GetDeivceContext();
	auto& app = D3DApp::GetInstance();

	gdc.IASetInputLayout(mpGraphicsPSO->mcpInputLayout.Get());
	gdc.IASetPrimitiveTopology(mpGraphicsPSO->mPrimitiveTopology);

	if (mpGraphicsPSO->mcpSampler != nullptr)
	{
		gdc.PSSetSamplers(0, 1, mpGraphicsPSO->mcpSampler.GetAddressOf());
	}

	gdc.VSSetShader(mpGraphicsPSO->mcpVertexShader.Get(), nullptr, 0);
	gdc.PSSetShader(mpGraphicsPSO->mcpPixelShader.Get(), nullptr, 0);
	if (app.IsDrawWire())
	{
		gdc.RSSetState(GraphicsPSOManager::GetInstance().GetRSWire().Get());
	}
	else
	{
		gdc.RSSetState(mpGraphicsPSO->mcpRS.Get());
	}
	LightingManager::GetInstance().UpdateConstantBuffer();
}

}
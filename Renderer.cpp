#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "GraphicsPSOManager.h"
#include "GraphicDeviceDX11.h"

using namespace jh::enums;
using namespace jh::graphics;
namespace jh
{


Renderer::Renderer(const jh::enums::eRenererComponentType eRendererType)
	: Component(eComponentType::RENDERER)
	, meRendererType(eRendererType)
{
}

void Renderer::ShadowRender()
{
	GetOwner().GetTransform().UpdateConstantBuffer();
	mpModel->ShadowRender();
}

void Renderer::Render()
{
	assert(mpModel != nullptr);
	GetOwner().GetTransform().UpdateConstantBuffer();
	SetIBLShaderResourceViews();
	mpModel->Render();
}
void Renderer::SetIBLShaderResourceViews()
{
	auto& psoManager = GraphicsPSOManager::GetInstance();
	auto& gd = GraphicDeviceDX11::GetInstance().GetDeivceContext();
	ID3D11ShaderResourceView* pSrvs[2] =
	{
		psoManager.mCubeMapping.cpCubeMapDiffuseIBLSRV.Get(),
		psoManager.mCubeMapping.cpCubeMapSepcularIBLSRV.Get()
	};
	gd.PSSetShaderResources(4, 2, pSrvs);
}
}
#include "MeshRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "GraphicDeviceDX11.h"
#include "D3DApp.h"


using namespace jh::enums;
using namespace jh::graphics;

namespace jh
{


MeshRenderer::MeshRenderer()
	: Renderer(eRenererComponentType::MESH_RENDERER)
{
		
}
void MeshRenderer::Render()
{
	assert(mpMesh != nullptr && mpMaterial != nullptr);
	GetOwner().GetTransform().UpdateConstantBuffer();
	mpMaterial->SetPipeline();
	mpMesh->Render(); 
	if (D3DApp::GetInstance().IsDrawNormal())
	{
		mpMesh->DebugNormalRender();
	}
	mpMaterial->ClearPipeline();
}



}
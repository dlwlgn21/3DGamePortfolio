#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "D3DApp.h"
#include "GraphicDeviceDX11.h"
#include "GraphicsPSOManager.h"
#include "ShadowManager.h"

static constexpr const UINT MESH_MAX_COUNT = 8;
using namespace jh::enums;
using namespace jh::graphics;
using namespace DirectX;
using namespace DirectX::SimpleMath;
namespace jh::graphics
{

Model::Model()
	: GraphicResource(eResourceType::MODEL)
	, meModelType(eModelType::NON_SKINNED_MESH_MODEL)
	, mpMaterial(nullptr)
	, mpBoneAnimator(nullptr)

{
	mpMeshes.reserve(MESH_MAX_COUNT);
}
void Model::SetMeshes(std::vector<Mesh*>& pMeshes)
{
	for (auto* pMesh : pMeshes)
	{
		assert(pMesh != nullptr);
	}
	mpMeshes = pMeshes;
}
void Model::SetMaterial(jh::graphics::Material& material)
{
	mpMaterial = &material;
}

void Model::SetBoneAnimator(jh::BoneAnimator& boneAnimator)
{
	mpBoneAnimator = &boneAnimator;
}

void Model::ShadowRender()
{
	for (size_t i = 0; i < mpMeshes.size(); ++i)
	{
		mpMeshes[i]->Render();
	}
}
void Model::Render()
{
	assert(mpMeshes.size() != 0);
	for (size_t i = 0; i < mpMeshes.size(); ++i)
	{
		assert(mpMeshes[i] != nullptr);
		mpMaterial->SetPipeline();
		ShadowManager::GetInstance().SetShadowSRV();
		mpMeshes[i]->Render();
		ShadowManager::GetInstance().ClearShadowSRV();
		if (D3DApp::GetInstance().IsDrawNormal())
		{
			mpMeshes[i]->DebugNormalRender();
		}
	}
}

}

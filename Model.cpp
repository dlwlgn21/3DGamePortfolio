#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "D3DApp.h"

using namespace jh::enums;

static constexpr const UINT MESH_MAX_COUNT = 8;

namespace jh::graphics
{

Model::Model()
	: GraphicResource(eResourceType::MODEL)
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
void Model::SetMaterials(std::vector<Material*>& pMaterials)
{
	for (auto* pMaterial : pMaterials)
	{
		assert(pMaterial != nullptr);
	}
	mpMaterials = pMaterials;
}
void Model::Render()
{
	assert(mpMeshes.size() != 0);
	for (size_t i = 0; i < mpMeshes.size(); ++i)
	{
		assert(mpMaterials[i] != nullptr && mpMeshes[i] != nullptr);
		mpMaterials[i]->SetPipeline();
		mpMeshes[i]->Render();
		if (D3DApp::GetInstance().IsDrawNormal())
		{
			mpMeshes[i]->DebugNormalRender();
		}
		mpMaterials[i]->ClearPipeline();
	}
}
}
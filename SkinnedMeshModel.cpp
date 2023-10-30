#include "SkinnedMeshModel.h"
#include "Material.h"
#include "Mesh.h"
#include "D3DApp.h"

using namespace DirectX::SimpleMath;

namespace jh::graphics
{

SkinnedMeshModel::SkinnedMeshModel()
	: Model()
{
}

void SkinnedMeshModel::Render()
{
	assert(mpMeshes.size() != 0);
	for (size_t i = 0; i < mpMeshes.size(); ++i)
	{
		assert(mpMeshes[i] != nullptr);
		if (mpBoneAnimator != nullptr)
		{
			mpBoneAnimator->UpdateDyanmicStructuredAnimationBuffer();
		}

		mpMaterials[0]->SetPipeline();
		mpMeshes[i]->Render();
		if (D3DApp::GetInstance().IsDrawNormal())
		{
			mpMeshes[i]->DebugNormalRender();
		}
		mpMaterials[0]->ClearPipeline();
	}
}


}
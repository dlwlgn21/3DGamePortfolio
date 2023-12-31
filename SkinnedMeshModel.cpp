#include "SkinnedMeshModel.h"
#include "Material.h"
#include "Mesh.h"
#include "D3DApp.h"
#include "ShadowManager.h"

using namespace DirectX::SimpleMath;

namespace jh::graphics
{

SkinnedMeshModel::SkinnedMeshModel()
	: Model()
{
}

void SkinnedMeshModel::ShadowRender()
{
	assert(mpMeshes.size() != 0);
	for (size_t i = 0; i < mpMeshes.size(); ++i)
	{
		assert(mpMeshes[i] != nullptr);
		if (mpBoneAnimator != nullptr)
		{
			mpBoneAnimator->UpdateDyanmicStructuredAnimationBuffer();
		}
		mpMeshes[i]->Render();
	}
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
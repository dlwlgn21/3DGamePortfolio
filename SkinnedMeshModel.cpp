#include "SkinnedMeshModel.h"

using namespace DirectX::SimpleMath;

namespace jh::graphics
{

SkinnedMeshModel::SkinnedMeshModel()
	: Model()
{
}

void SkinnedMeshModel::InitAnimationDataAndStructuredBuffer(jh::graphics::AnimationData* pAnimData)
{
	assert(pAnimData != nullptr);
	if (pAnimData->ClipMap.size() == 0)
	{
		assert(false);
	}
	mpAnimData = pAnimData;
	auto& matricices = mBoneTransformMatrices.GetCPUBuffer();
	matricices.resize(mpAnimData->BoneTransformMatrixArray.size());
	for (UINT i = 0; i < mpAnimData->BoneTransformMatrixArray.size(); ++i)
	{
		matricices[i] = Matrix();
	}
	mBoneTransformMatrices.CreateStructuredBuffer(static_cast<UINT>(matricices.size()));
}


void SkinnedMeshModel::prepareBoneTransfromMatrices(const std::string& clipKey, const int frame)
{
	mpAnimData->PrepareAllBoneTransformMatrices(clipKey, frame);
	auto& matricices = mBoneTransformMatrices.GetCPUBuffer();
	for (int boneIndex = 0; boneIndex < matricices.size(); ++boneIndex)
	{
		matricices[boneIndex] = mpAnimData->GetFinalTransformMatrixRow(boneIndex, frame).Transpose();
	}
}
void SkinnedMeshModel::Render(const std::string& clipKey, const int frame)
{
	prepareBoneTransfromMatrices(clipKey, frame);
	mBoneTransformMatrices.UploadGPUBuffer(0); 
	Model::Render();
}


}
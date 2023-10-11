#include "SkinnedMeshModel.h"

using namespace DirectX::SimpleMath;

namespace jh::graphics
{

SkinnedMeshModel::SkinnedMeshModel()
	: Model()
{
	ZeroMemory(&mAnimData, sizeof(jh::graphics::AnimationData));
}

void SkinnedMeshModel::InitAnimationDataAndStructuredBuffer(jh::graphics::AnimationData& animData)
{
	if (animData.ClipArray.empty())
	{
		assert(false);
	}
	mAnimData = animData;
	auto& cpuBuffer = mBoneTransformMatrices.GetCPUBuffer();
	cpuBuffer.resize(mAnimData.ClipArray.front().KeyBoneAndFrame2DArrays.size());
	for (UINT i = 0; i < mAnimData.ClipArray.front().KeyBoneAndFrame2DArrays.size(); ++i)
	{
		cpuBuffer[i] = Matrix();
	}
	mBoneTransformMatrices.Create(static_cast<UINT>(cpuBuffer.size()));
}


void SkinnedMeshModel::updateBoneTransfromMatrices(const int clipIndex, const int frame)
{
	mAnimData.UpdateBoneTransformMatrices(clipIndex, frame);
	auto& matricices = mBoneTransformMatrices.GetCPUBuffer();
	for (int boneIndex = 0; boneIndex < matricices.size(); ++boneIndex)
	{
		matricices[boneIndex] = mAnimData.GetFinalTransformMatrix(clipIndex, boneIndex, frame).Transpose();
	}
	mBoneTransformMatrices.UpdateGPUBuffer(0);
}
void SkinnedMeshModel::Render(const int clipIndex, const int frame)
{
	updateBoneTransfromMatrices(clipIndex, frame);
	Model::Render();
}


}
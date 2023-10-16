#include "BoneAnimator.h"

using namespace jh::enums;
using namespace jh::graphics;
namespace jh
{

BoneAnimator::BoneAnimator()
	: Component(jh::enums::eComponentType::ANIMATOR)
	, mpAnimData(nullptr)
	, mBoneTransformMatrices()
	, mCurrentClipKey(nullptr)
	, mPrevClipKey(nullptr)
	, mFrameCount(0)
{
}
void BoneAnimator::Update()
{
	if (IsCurrentAnimClipLastFrame())
	{
		mFrameCount = 0;
	}
	++mFrameCount;
}
void BoneAnimator::FixedUpdate()
{
	assert(mCurrentClipKey != nullptr);
	prepareBoneTransfromMatrices(mFrameCount);
	mBoneTransformMatrices.UploadGPUBuffer(0);
}

void BoneAnimator::ChangeCurrentAnimationClip(const std::string* pKey)
{
	assert(mCurrentClipKey != nullptr && pKey != nullptr);
	mFrameCount = 0;
	mPrevClipKey = mCurrentClipKey;
	mCurrentClipKey = pKey;
}
void BoneAnimator::InitAnimationData(jh::graphics::AnimationData* pAnimData, const eAnimClipKeyContainerType eKeyContainerType)
{
	assert(pAnimData != nullptr && mpAnimData == nullptr && !pAnimData->ClipMap.empty());
	mpAnimData = pAnimData;
	auto& matricices = mBoneTransformMatrices.GetCPUBuffer();
	matricices.resize(mpAnimData->BoneTransformMatrixArray.size());
	for (UINT i = 0; i < mpAnimData->BoneTransformMatrixArray.size(); ++i)
	{
		matricices[i] = Matrix();
	}
	mBoneTransformMatrices.CreateStructuredBuffer(static_cast<UINT>(matricices.size()));
	mCurrentClipKey = &AnimationDataManager::GetInstance().GetAnimationCilpKeys(eKeyContainerType)[0];
	mPrevClipKey = mCurrentClipKey;
}

const bool BoneAnimator::IsCurrentAnimClipLastFrame()
{
	return mpAnimData->ClipMap[*mCurrentClipKey].IsLastFrame(mFrameCount) ? true : false;
}

void BoneAnimator::prepareBoneTransfromMatrices(const int frame)
{
	mpAnimData->PrepareAllBoneTransformMatrices(*mCurrentClipKey, frame);
	auto& matricices = mBoneTransformMatrices.GetCPUBuffer();
	for (int boneIndex = 0; boneIndex < matricices.size(); ++boneIndex)
	{
		matricices[boneIndex] = mpAnimData->GetFinalTransformMatrixRow(boneIndex, frame).Transpose();
	}
}

}
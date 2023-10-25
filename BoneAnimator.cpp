#include "BoneAnimator.h"

#include "Time.h"

using namespace jh::enums;
using namespace jh::graphics;
namespace jh
{

BoneAnimator::BoneAnimator()
	: Component(jh::enums::eComponentType::ANIMATOR)
	, mpAnimData(nullptr)
	, mBoneTransformMatrices()
	, mpCurrentClipKey(nullptr)
	, mCurrentAnimAccumTime(0.0f)
{
}
void BoneAnimator::Update()
{
	if (IsCurrentAnimClipLastFrame())
	{
		mCurrentAnimAccumTime = 0.0f;
	}
	mCurrentAnimAccumTime += Time::DeltaTime();
}
void BoneAnimator::FixedUpdate()
{
	assert(mpCurrentClipKey != nullptr);
	prepareBoneTransfromMatrices(mCurrentAnimAccumTime);
	mBoneTransformMatrices.UploadGPUBuffer(0);
}
void BoneAnimator::UpdateDyanmicStructuredAnimationBuffer()
{
	assert(mpCurrentClipKey != nullptr);
	prepareBoneTransfromMatrices(mCurrentAnimAccumTime);
	mBoneTransformMatrices.UploadGPUBuffer(0);
}

void BoneAnimator::ChangeCurrentAnimationClip(const std::string* pKey)
{
	assert(mpCurrentClipKey != nullptr && pKey != nullptr);
	mCurrentAnimAccumTime = 0.0f;
	mpCurrentClipKey = pKey;
}
void BoneAnimator::InitAnimationData(jh::graphics::AnimationData* pAnimData, const eAnimClipKeyContainerType eKeyContainerType)
{
	assert(pAnimData != nullptr && mpAnimData == nullptr && !pAnimData->ClipMap.empty());
	mpAnimData = pAnimData;
	auto& matricices = mBoneTransformMatrices.GetCPUBuffer();
	matricices.resize(mpAnimData->BoneTransformMatrixArray.size());
	for (UINT i = 0; i < mpAnimData->BoneTransformMatrixArray.size(); ++i)
	{
		matricices[i] = Matrix::Identity;
	}
	mBoneTransformMatrices.CreateStructuredBuffer(static_cast<UINT>(matricices.size()));
	mpCurrentClipKey = &AnimationDataManager::GetInstance().GetAnimationCilpKeys(eKeyContainerType)[0];
}

const bool BoneAnimator::IsCurrentAnimClipLastFrame()
{
	return mpAnimData->ClipMap[*mpCurrentClipKey].IsLastFrame(mCurrentAnimAccumTime);
}

void BoneAnimator::prepareBoneTransfromMatrices(const float currentAccumTime)
{
	mpAnimData->PrepareAllBoneTransformMatrices(*mpCurrentClipKey, currentAccumTime);
	auto& matricices = mBoneTransformMatrices.GetCPUBuffer();
	for (int boneIndex = 0; boneIndex < matricices.size(); ++boneIndex)
	{
		matricices[boneIndex] = mpAnimData->GetFinalTransformMatrixRow(boneIndex).Transpose();
	}
}

}
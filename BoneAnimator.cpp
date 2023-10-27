#include "BoneAnimator.h"

#include "Time.h"

using namespace jh::enums;
using namespace jh::graphics;
namespace jh
{

BoneAnimator::BoneAnimator()
	: Component(jh::enums::eComponentType::ANIMATOR)
	, mpAnimData(nullptr)
	, mBoneTransforms()
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

void BoneAnimator::UpdateDyanmicStructuredAnimationBuffer()
{
	assert(mpCurrentClipKey != nullptr);
	prepareBoneTransfroms(mCurrentAnimAccumTime);
	mBoneTransforms.UploadGPUBuffer(0);
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
	auto& matricices = mBoneTransforms.GetCPUBuffer();
	matricices.resize(mpAnimData->BoneTransformArray.size());
	for (UINT i = 0; i < mpAnimData->BoneTransformArray.size(); ++i)
	{
		matricices[i] = Matrix::Identity;
	}
	mBoneTransforms.CreateStructuredBuffer(static_cast<UINT>(matricices.size()));
	mpCurrentClipKey = &AnimationDataManager::GetInstance().GetAnimationCilpKeys(eKeyContainerType)[0];
}

const float BoneAnimator::GetCurrentPlayingClipPercentage()
{
	float retTime = mCurrentAnimAccumTime / mpAnimData->ClipMap[*mpCurrentClipKey].TotalAnimTime;
	assert(retTime >= 0.0f && retTime <= 1.0f);
	return retTime;
}

const bool BoneAnimator::IsCurrentAnimClipLastFrame()
{
	return mpAnimData->ClipMap[*mpCurrentClipKey].IsLastFrame(mCurrentAnimAccumTime);
}

void BoneAnimator::prepareBoneTransfroms(const float currentAccumTime)
{
	mpAnimData->PrepareAllBoneTransforms(*mpCurrentClipKey, currentAccumTime);
	auto& matricices = mBoneTransforms.GetCPUBuffer();
	for (int boneIndex = 0; boneIndex < matricices.size(); ++boneIndex)
	{
		matricices[boneIndex] = mpAnimData->GetFinalTransformMatrixRow(boneIndex).Transpose();
	}
}


}
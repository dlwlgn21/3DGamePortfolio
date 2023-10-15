#include "BoneAnimator.h"


using namespace jh::enums;
using namespace jh::graphics;
namespace jh
{
	BoneAnimator::BoneAnimator()
		: Component(jh::enums::eComponentType::ANIMATOR)
	{
	}
	void BoneAnimator::Update()
	{
	}
	void BoneAnimator::FixedUpdate()
	{
	}
	void BoneAnimator::InitAnimationData(jh::graphics::AnimationData* pAnimData)
	{
		assert(pAnimData != nullptr && mpAnimData != nullptr);
		if (pAnimData->ClipArray.empty())
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
	void BoneAnimator::prepareBoneTransfromMatrices(const int clipIndex, const int frame)
	{
		mpAnimData->PrepareAllBoneTransformMatrices(clipIndex, frame);
		auto& matricices = mBoneTransformMatrices.GetCPUBuffer();
		for (int boneIndex = 0; boneIndex < matricices.size(); ++boneIndex)
		{
			matricices[boneIndex] = mpAnimData->GetFinalTransformMatrixRow(boneIndex, frame).Transpose();
		}
	}
}
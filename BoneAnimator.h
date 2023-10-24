#pragma once
#include "Component.h"
#include "AnimationData.h"
#include "DynamicStructuredBuffer.h"
#include "AnimationDataManager.h"

namespace jh
{

class BoneAnimator final : public Component
{
public:
	enum class eCharacterAnimState
	{
		IDLE,
		WALK,
		COUNT
	};
public:
	BoneAnimator();
	virtual ~BoneAnimator() = default;

	void Update();
	void FixedUpdate();

	void ChangeCurrentAnimationClip(const std::string* key);
	void InitAnimationData(jh::graphics::AnimationData* pAnimData, const eAnimClipKeyContainerType eKeyContainerType);
	const bool IsCurrentAnimClipLastFrame();
	jh::graphics::AnimationData& GetAnimData() { assert(mpAnimData != nullptr); return *mpAnimData; }
private:
	void prepareBoneTransfromMatrices(const int frame);
private:
	jh::graphics::AnimationData*										mpAnimData;
	jh::graphics::DynamicStructuredBuffer<DirectX::SimpleMath::Matrix>	mBoneTransformMatrices;
	const std::string*													mpCurrentClipKey;
	int																	mFrameCount;
};

}


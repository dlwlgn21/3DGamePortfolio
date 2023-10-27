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
		BACK_WALK,
		STRAFE_RIGHT,
		STRAFE_LEFT,
		ROLL,
		KICK,
		ATTACK_SLASH_1,
		ATTACK_SLASH_2,
		ATTACK_SLASH_3,
		COUNT
	};
	enum class eMonsterAnimState
	{
		IDLE,
		WALK,
		RUN,
		ROARING,
		ROLL,
		SLASH_ATTACK,
		JUMP_ATTACK,
		COUNT
	};

public:
	BoneAnimator();
	virtual ~BoneAnimator() = default;

	void Update() override;

	void UpdateDyanmicStructuredAnimationBuffer();

	void ChangeCurrentAnimationClip(const std::string* key);
	void InitAnimationData(jh::graphics::AnimationData* pAnimData, const eAnimClipKeyContainerType eKeyContainerType);
	const float GetCurrentPlayingClipPercentage();
	const bool IsCurrentAnimClipLastFrame();
	jh::graphics::AnimationData& GetAnimData() { assert(mpAnimData != nullptr); return *mpAnimData; }
	

private:
	void prepareBoneTransfroms(const float currentAccumTime);
private:
	jh::graphics::AnimationData*										mpAnimData;
	jh::graphics::DynamicStructuredBuffer<DirectX::SimpleMath::Matrix>	mBoneTransforms;
	const std::string*													mpCurrentClipKey;
	float																mCurrentAnimAccumTime;
};

}


#pragma once
#include "Script.h"
#include "BoneAnimator.h"

namespace jh
{

enum ePlayerState
{
	IDLE,
	WALK,
};

class PlayerScript final : public Script
{
public:
	PlayerScript();
	virtual ~PlayerScript() = default;

	void Initialize() override;
	void Update() override;
	void ChangeAnimationClip(const BoneAnimator::eCharacterAnimState eAnimState);

	void SetBoneAnimator(BoneAnimator* pAniamtor) { assert(mpAnimator == nullptr && pAniamtor != nullptr); mpAnimator = pAniamtor; }
private:
	BoneAnimator* mpAnimator;
	BoneAnimator::eCharacterAnimState meAnimState;
};

}
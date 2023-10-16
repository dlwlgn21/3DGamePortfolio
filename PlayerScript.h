#pragma once
#include "Script.h"
#include "BoneAnimator.h"
#include "State.h"
#include "StateMachine.h"

namespace jh
{

enum ePlayerState
{
	IDLE,
	WALK,
	COUNT
};

class PlayerScript final : public Script
{
public:
	PlayerScript();
	virtual ~PlayerScript();

	void Initialize() override;
	void Update() override;


	void SetBoneAnimator(BoneAnimator* pAniamtor) { assert(mpAnimator == nullptr && pAniamtor != nullptr); mpAnimator = pAniamtor; }


	const BoneAnimator::eCharacterAnimState GetCurrAnimState()		const { return meAnimState; }
	const ePlayerState						GetCurrPlayerState()	const { return mePlayerState; }
	BoneAnimator&							GetAnimator()				  { return *mpAnimator; }


	void ChangeAnimationClip(const BoneAnimator::eCharacterAnimState eAnimState);
	void ChangePlayerState(const ePlayerState eState);

private:
	jh::fsm::StateMachine<PlayerScript>		mFSM;
	jh::fsm::State<PlayerScript>*			mpStates[static_cast<UINT>(ePlayerState::COUNT)];
	BoneAnimator*							mpAnimator;

	BoneAnimator::eCharacterAnimState		meAnimState;
	ePlayerState							mePlayerState;
};

}
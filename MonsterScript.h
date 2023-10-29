#pragma once
#include "Script.h"
#include "BoneAnimator.h"
#include "State.h"
#include "StateMachine.h"

namespace jh
{
enum class eMonsterState
{
	IDLE,
	SEARCHING,
	TRACING,
	ATTACKING,
	HITTED,
	DEAD,
	COUNT
};

class MonsterScript final : public Script
{
public:
	MonsterScript();
	virtual ~MonsterScript();

	void Initialize() override;
	void Update() override;

	void SetBoneAnimator(BoneAnimator* pAniamtor) { assert(mpAnimator == nullptr && pAniamtor != nullptr); mpAnimator = pAniamtor; }


	const BoneAnimator::eMonsterAnimState	GetCurrAnimState()		const { return meAnimState; }
	const eMonsterState						GetCurrMonsterState()	const { return meMonsterState; }
	BoneAnimator& GetAnimator() { return *mpAnimator; }


	void ChangeAnimationClip(const BoneAnimator::eMonsterAnimState eAnimState);
	void ChangeMonsterState(const eMonsterState eState);

	const float GetWalkSpeed() { return mWalkSpeed; }
	const float GetTurnRoationSpeedDeg() { return mTurnRotationSpeedDeg; }

private:
	jh::fsm::StateMachine<MonsterScript>		mFSM;
	jh::fsm::State<MonsterScript>* mpStates[static_cast<UINT>(eMonsterState::COUNT)];
	BoneAnimator* mpAnimator;

	BoneAnimator::eMonsterAnimState			meAnimState;
	eMonsterState							meMonsterState;

	float mWalkSpeed;
	float mTurnRotationSpeedDeg;
};

}
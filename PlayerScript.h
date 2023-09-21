#pragma once
#include "Script.h"
#include "StateMachine.h"
#include "State.h"

namespace jh
{

enum ePlayerState
{
	IDLE,
	RUNNING,
	ROLLING,
	DASH,
	ATTACK_1,
	ATTACK_2,
	ATTACK_3,
	ATTACK_PUSH,
	HITTED,
	COUNT
};

class Animator;
class PlayerScript final : public Script
{
public:
	PlayerScript();
	virtual ~PlayerScript();

	void Initialize() override;
	void Update() override;
	void ChangeState(const ePlayerState eState);
	void ChangeAnimation(const std::string& key);

public:
	void DashAnimStart();
	void DashAnimComplete();

	void RollingAnimStart();
	void RollingAnimComplete();

	void AttackOneAnimStart();
	void AttackOneAnimComplete();

	void AttackTwoAnimStart();
	void AttackTwoAnimComplete();

	void AttackThreeAnimStart();
	void AttackThreeAnimComplete();

	void AttackPushAnimStart();
	void AttackPushAnimComplete();

	void HittedAnimStart();
	void HittedAnimComplete();

private:
	void initAnimationEvent();
	void AttackAnimCompleteCallBack();
	void DodgeAnimCompleteCallBack();
	void HittedAnimStartCallBack();
	void HittedAnimCompleteCallBack();

private:
	jh::fsm::StateMachine<PlayerScript>				mFSM;
	ePlayerState									meCurrentState;
	jh::fsm::State<PlayerScript>*					mpStates[static_cast<UINT>(ePlayerState::COUNT)];
	Animator*										mpAnimator = nullptr;
};


}
#include "PlayerScript.h"
#include "PlayerState.h"

#include "Animator.h"

using namespace jh::fsm;
using namespace jh::enums;

namespace jh
{

PlayerScript::PlayerScript()
	: Script()
	, mFSM()
	, meCurrentState(ePlayerState::IDLE)
	, mpStates()
{
	const UINT STATE_COUNT = static_cast<UINT>(ePlayerState::COUNT);
	mpStates[static_cast<UINT>(ePlayerState::IDLE)]			= new PlayerIdleState(this);
	mpStates[static_cast<UINT>(ePlayerState::RUNNING)]		= new PlayerRunningState(this);
	mpStates[static_cast<UINT>(ePlayerState::ROLLING)]		= new PlayerRollingState(this);
	mpStates[static_cast<UINT>(ePlayerState::DASH)]			= new PlayerDashState(this);

	mpStates[static_cast<UINT>(ePlayerState::ATTACK_1)]		= new PlayerAttackOneState(this);
	mpStates[static_cast<UINT>(ePlayerState::ATTACK_2)]		= new PlayerAttackTwoState(this);
	mpStates[static_cast<UINT>(ePlayerState::ATTACK_3)]		= new PlayerAttackThreeState(this);
	mpStates[static_cast<UINT>(ePlayerState::ATTACK_PUSH)]	= new PlayerAtttackPushState(this);

	mpStates[static_cast<UINT>(ePlayerState::HITTED)]		= new PlayerHittedState(this);
	mFSM.Init(*mpStates[static_cast<UINT>(ePlayerState::IDLE)]);
}

PlayerScript::~PlayerScript()
{
	for (auto* pState : mpStates)
	{
		if (pState != nullptr)
		{
			delete pState;
			pState = nullptr;
		}
	}
}

void PlayerScript::Initialize()
{
	mpAnimator = static_cast<Animator*>(GetOwner().GetComponentOrNull(eComponentType::ANIMATOR));
	assert(mpAnimator != nullptr);
	initAnimationEvent();
}

void PlayerScript::Update()
{
	mFSM.Excute();
}

void PlayerScript::ChangeState(const ePlayerState eState)
{
	assert(eState != meCurrentState); 
	meCurrentState = eState;
	mFSM.ChangeState(*mpStates[static_cast<UINT>(eState)]);
}

void PlayerScript::ChangeAnimation(const std::string& key)
{
	switch (meCurrentState)
	{
	case IDLE:
	case RUNNING:
	{
		mpAnimator->PlayAnimation(key, true);
		break;
	}
	case ATTACK_1:
	case ATTACK_2:
	case ATTACK_3:
	case ATTACK_PUSH:
	case DASH:
	case ROLLING:
	case HITTED:
	{
		mpAnimator->PlayAnimationWithReset(key, false);
		break;
	}
	default:
		assert(false);
		break;
	}
}


void PlayerScript::DashAnimStart()
{

}
void PlayerScript::DashAnimComplete()
{
	DodgeAnimCompleteCallBack();
}

void PlayerScript::RollingAnimStart()
{

}
void PlayerScript::RollingAnimComplete()
{
	DodgeAnimCompleteCallBack();
}


void PlayerScript::AttackOneAnimStart()
{

}
void PlayerScript::AttackOneAnimComplete()
{
	AttackAnimCompleteCallBack();
}


void PlayerScript::AttackTwoAnimStart()
{

}
void PlayerScript::AttackTwoAnimComplete()
{
	AttackAnimCompleteCallBack();
}


void PlayerScript::AttackThreeAnimStart()
{

}
void PlayerScript::AttackThreeAnimComplete()
{
	AttackAnimCompleteCallBack();
}


void PlayerScript::AttackPushAnimStart()
{

}

void PlayerScript::AttackPushAnimComplete()
{
	AttackAnimCompleteCallBack();
}

void PlayerScript::HittedAnimStart()
{
	HittedAnimStartCallBack();
}
void PlayerScript::HittedAnimComplete()
{
	HittedAnimCompleteCallBack();
}

void PlayerScript::initAnimationEvent()
{
	assert(mpAnimator != nullptr);

	mpAnimator->GetStartEvent(Player::PLAYER_ROLLING_KEY) = std::bind(&PlayerScript::RollingAnimStart, this);
	mpAnimator->GetCompleteEvent(Player::PLAYER_ROLLING_KEY) = std::bind(&PlayerScript::RollingAnimComplete, this);

	mpAnimator->GetStartEvent(Player::PLAYER_DASH_KEY) = std::bind(&PlayerScript::DashAnimStart, this);
	mpAnimator->GetCompleteEvent(Player::PLAYER_DASH_KEY) = std::bind(&PlayerScript::DashAnimComplete, this);

	mpAnimator->GetStartEvent(Player::ATTACK_1_ANIM_KEY) = std::bind(&PlayerScript::AttackOneAnimStart, this);
	mpAnimator->GetCompleteEvent(Player::ATTACK_1_ANIM_KEY) = std::bind(&PlayerScript::AttackOneAnimComplete, this);

	mpAnimator->GetStartEvent(Player::ATTACK_2_ANIM_KEY) = std::bind(&PlayerScript::AttackTwoAnimStart, this);
	mpAnimator->GetCompleteEvent(Player::ATTACK_2_ANIM_KEY) = std::bind(&PlayerScript::AttackTwoAnimComplete, this);

	mpAnimator->GetStartEvent(Player::ATTACK_3_ANIM_KEY) = std::bind(&PlayerScript::AttackThreeAnimStart, this);
	mpAnimator->GetCompleteEvent(Player::ATTACK_3_ANIM_KEY) = std::bind(&PlayerScript::AttackThreeAnimComplete, this);

	mpAnimator->GetStartEvent(Player::ATTACK_PUSH_ANIM_KEY) = std::bind(&PlayerScript::AttackPushAnimStart, this);
	mpAnimator->GetCompleteEvent(Player::ATTACK_PUSH_ANIM_KEY) = std::bind(&PlayerScript::AttackPushAnimComplete, this);

	mpAnimator->GetStartEvent(Player::PLAYER_HITTED_KEY) = std::bind(&PlayerScript::HittedAnimStart, this);
	mpAnimator->GetCompleteEvent(Player::PLAYER_HITTED_KEY) = std::bind(&PlayerScript::HittedAnimComplete, this);
}

void PlayerScript::AttackAnimCompleteCallBack()
{
	assert(meCurrentState != ePlayerState::IDLE && meCurrentState != ePlayerState::RUNNING);
	static_cast<PlayerAttackState*>(mpStates[static_cast<UINT>(meCurrentState)])->AttckAnimationCompletedCallBack();
}
void PlayerScript::DodgeAnimCompleteCallBack()
{
	assert(meCurrentState == ePlayerState::ROLLING || meCurrentState == ePlayerState::DASH);
	static_cast<PlayerState*>(mpStates[static_cast<UINT>(meCurrentState)])->DodgeAnimationCompletedCallBack();
}

void PlayerScript::HittedAnimStartCallBack()
{
	assert(meCurrentState == ePlayerState::HITTED);
	static_cast<PlayerState*>(mpStates[static_cast<UINT>(meCurrentState)])->HittedAnimationStartCallBack();
}
void PlayerScript::HittedAnimCompleteCallBack()
{
	assert(meCurrentState == ePlayerState::HITTED);
	static_cast<PlayerState*>(mpStates[static_cast<UINT>(meCurrentState)])->HittedAnimationCompletedCallBack();
}

}
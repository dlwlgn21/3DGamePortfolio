#pragma once
#include "State.h"

#include "InputManager.h"
#include "Time.h"

#include "GameObject.h"
#include "Transform.h"
#include "PlayerScript.h"
#include "BoneAnimator.h"

namespace jh::fsm
{
class PlayerState : public State<PlayerScript>
{
public:
	PlayerState(PlayerScript* pPlayerScript)
		: State(pPlayerScript)
	{
	}
	virtual ~PlayerState() = default;
	virtual void Enter() override
	{
		assert(false);
	}
	virtual void Excute() override
	{
		assert(false);
	}
	virtual void Exit() override
	{
		assert(false);
	}
	void ChangeAnimationClip(const BoneAnimator::eCharacterAnimState eAnimState)
	{
		mpOwnerScript->ChangeAnimationClip(eAnimState);
	}
	void ChangePlayerState(const ePlayerState eState)
	{
		mpOwnerScript->ChangePlayerState(eState);
	}
};


class PlayerIdleState : public PlayerState
{
public:
	PlayerIdleState(PlayerScript* pPlayerScript)
		: PlayerState(pPlayerScript)
	{

	}
	virtual ~PlayerIdleState() = default;
	void Enter() override
	{
	}

	void Excute() override
	{
		switch (mpOwnerScript->GetCurrAnimState())
		{
		case BoneAnimator::eCharacterAnimState::IDLE:
		{
			if (InputManager::GetKeyState(eKeyCode::UP) == eKeyState::PRESSED)
			{
				ChangeAnimationClip(BoneAnimator::eCharacterAnimState::IDLE_TO_WALK);
			}
			break;
		}
		case BoneAnimator::eCharacterAnimState::IDLE_TO_WALK:
		{
			if (mpOwnerScript->GetAnimator().IsCurrentAnimClipLastFrame())
			{
				ChangeAnimationClip(BoneAnimator::eCharacterAnimState::WALK_FORWARD);
				ChangePlayerState(ePlayerState::WALK);
			}
			break;
		}
		default:
			break;
		}
	}

	void Exit() override
	{
	}

};


class PlayerWalkingState : public PlayerState
{
public:
	PlayerWalkingState(PlayerScript* pPlayerScript)
		: PlayerState(pPlayerScript)
	{
	}
	virtual ~PlayerWalkingState() = default;

	void Enter() override
	{
	}

	void Excute() override
	{
		switch (mpOwnerScript->GetCurrAnimState())
		{
		case BoneAnimator::eCharacterAnimState::WALK_FORWARD:
		{
			auto& transform = mpOwnerScript->GetOwner().GetTransform();
			Vector3 moveVector = transform.GetPosition();
			if (InputManager::GetKeyState(eKeyCode::RIGHT) == eKeyState::PRESSED)
			{
				transform.AccumulateYaw(120.0f * Time::DeltaTime());
			}
			if (InputManager::GetKeyState(eKeyCode::LEFT) == eKeyState::PRESSED)
			{
				transform.AccumulateYaw(-120.0f * Time::DeltaTime());
			}
			moveVector += -transform.GetForwardRef() * 4.0f * Time::DeltaTime();
			transform.SetPosition(moveVector);
			if (mpOwnerScript->GetAnimator().IsCurrentAnimClipLastFrame())
			{
				if (InputManager::GetKeyState(eKeyCode::UP) != eKeyState::PRESSED)
				{
					ChangeAnimationClip(BoneAnimator::eCharacterAnimState::WALK_TO_IDLE);
				}
			}
			break;
		}
		case BoneAnimator::eCharacterAnimState::WALK_TO_IDLE:
		{
			if (mpOwnerScript->GetAnimator().IsCurrentAnimClipLastFrame())
			{
				ChangeAnimationClip(BoneAnimator::eCharacterAnimState::IDLE);
				ChangePlayerState(ePlayerState::IDLE);
			}
			break;
		}
		}
	}
	void Exit() override
	{
	}
};

//
//class PlayerRollingState : public PlayerState
//{
//public:
//	PlayerRollingState(PlayerScript* pPlayerScript)
//		: PlayerState(pPlayerScript)
//	{
//	}
//	virtual ~PlayerRollingState() = default;
//
//	void Enter() override
//	{
//		ChangeAnimation(Player::PLAYER_ROLLING_KEY);
//	}
//
//	void Excute() override
//	{
//	}
//
//	void Exit() override
//	{
//	}
//};
//
//class PlayerDashState : public PlayerState
//{
//public:
//	PlayerDashState(PlayerScript* pPlayerScript)
//		: PlayerState(pPlayerScript)
//	{
//	}
//	virtual ~PlayerDashState() = default;
//
//	void Enter() override
//	{
//		ChangeAnimation(Player::PLAYER_DASH_KEY);
//	}
//
//	void Excute() override
//	{
//	}
//
//	void Exit() override
//	{
//	}
//};
//
//class PlayerHittedState : public PlayerState
//{
//public:
//	PlayerHittedState(PlayerScript* pPlayerScript)
//		: PlayerState(pPlayerScript)
//	{
//	}
//	virtual ~PlayerHittedState() = default;
//
//	void Enter() override
//	{
//		ChangeAnimation(Player::PLAYER_HITTED_KEY);
//	}
//
//	void Excute() override
//	{
//	}
//
//	void Exit() override
//	{
//	}
//};
//
//#pragma region ATTACK_PART
//enum class ePlayerAttackType
//{
//	ATTACK_1,
//	ATTACK_2,
//	ATTACK_3,
//	ATTACK_PUSH,
//	COUNT
//};
//
//class PlayerAttackState : public PlayerState
//{
//public:
//	PlayerAttackState(PlayerScript* pPlayerScript, ePlayerAttackType eType)
//		: PlayerState(pPlayerScript)
//		, mePlayerAttackType(eType)
//	{
//	}
//	virtual ~PlayerAttackState() = default;
//	void Excute() override
//	{
//		CheckContinueAttack();
//	}
//	void Exit() override
//	{
//		ResetContinueAttackFlag();
//	}
//
//
//	void AttckAnimationCompletedCallBack()
//	{
//		if (mbIsContinueAttackFlag)
//		{
//			switch (mePlayerAttackType)
//			{
//			case jh::fsm::ePlayerAttackType::ATTACK_1:
//				ChangeState(ePlayerState::ATTACK_2);
//				break;
//			case jh::fsm::ePlayerAttackType::ATTACK_2:
//				ChangeState(ePlayerState::ATTACK_3);
//				break;
//			case jh::fsm::ePlayerAttackType::ATTACK_3:
//				ChangeState(ePlayerState::IDLE);
//				break;
//			default:
//				assert(false);
//				break;
//			}
//		}
//		else
//		{
//			ChangeState(ePlayerState::IDLE);
//		}
//	}
//
//	void CheckContinueAttack()
//	{
//		if (InputManager::GetKeyState(eKeyCode::Z) == eKeyState::DOWN)
//		{
//			mbIsContinueAttackFlag = true;
//		}
//	}
//
//	void ResetContinueAttackFlag()
//	{
//		mbIsContinueAttackFlag = false;
//	}
//
//protected:
//	ePlayerAttackType mePlayerAttackType;
//	bool mbIsContinueAttackFlag = false;
//};
//
//class PlayerAttackOneState : public PlayerAttackState
//{
//public:
//	PlayerAttackOneState(PlayerScript* pPlayerScript)
//		: PlayerAttackState(pPlayerScript, ePlayerAttackType::ATTACK_1)
//	{
//	}
//	virtual ~PlayerAttackOneState() = default;
//
//	void Enter() override
//	{
//		ChangeAnimation(Player::ATTACK_1_ANIM_KEY);
//	}
//};
//
//class PlayerAttackTwoState : public PlayerAttackState
//{
//public:
//	PlayerAttackTwoState(PlayerScript* pPlayerScript)
//		: PlayerAttackState(pPlayerScript, ePlayerAttackType::ATTACK_2)
//	{
//	}
//	virtual ~PlayerAttackTwoState() = default;
//
//	void Enter() override
//	{
//		ChangeAnimation(Player::ATTACK_2_ANIM_KEY);
//	}
//};
//
//class PlayerAttackThreeState : public PlayerAttackState
//{
//public:
//	PlayerAttackThreeState(PlayerScript* pPlayerScript)
//		: PlayerAttackState(pPlayerScript, ePlayerAttackType::ATTACK_3)
//	{
//	}
//	virtual ~PlayerAttackThreeState() = default;
//
//	void Enter() override
//	{
//		ChangeAnimation(Player::ATTACK_3_ANIM_KEY);
//	}
//};
//class PlayerAtttackPushState : public PlayerAttackState
//{
//public:
//	PlayerAtttackPushState(PlayerScript* pPlayerScript)
//		: PlayerAttackState(pPlayerScript, ePlayerAttackType::ATTACK_PUSH)
//	{
//	}
//	virtual ~PlayerAtttackPushState() = default;
//
//	void Enter() override
//	{
//		ChangeAnimation(Player::ATTACK_PUSH_ANIM_KEY);
//	}
//	void Excute() override
//	{
//		// NONE
//	}
//};
#pragma endregion

}
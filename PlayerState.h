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
				ChangeAnimationClip(BoneAnimator::eCharacterAnimState::WALK);
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
		case BoneAnimator::eCharacterAnimState::WALK:
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
			if (InputManager::GetKeyState(eKeyCode::UP) != eKeyState::PRESSED)
			{
				ChangeAnimationClip(BoneAnimator::eCharacterAnimState::IDLE);
				ChangePlayerState(ePlayerState::IDLE);
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
}
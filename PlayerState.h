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
		, mWalkSpeed(pPlayerScript->GetWalkSpeed())
		, mTurnRotationSpeedDeg(pPlayerScript->GetTurnRoationSpeedDeg())
		, mpTransform(&pPlayerScript->GetOwner().GetTransform())
		, mpAnimator(&pPlayerScript->GetAnimator())
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

protected:
	const bool IsChangeIdleState()
	{
		if (!InputManager::IsAnyKeyPressed())
		{
			ChangeAnimationClip(BoneAnimator::eCharacterAnimState::IDLE);
			ChangePlayerState(ePlayerState::IDLE);
			return true;
		}
		return false;
	}
	const bool IsChangeWalkState()
	{
		if (InputManager::GetKeyState(eKeyCode::UP) == eKeyState::PRESSED)
		{
			ChangeAnimationClip(BoneAnimator::eCharacterAnimState::WALK);
			ChangePlayerState(ePlayerState::WALK);
			return true;
		}
		return false;
	}
	const bool IsChangeStrafeRightState()
	{
		if (InputManager::GetKeyState(eKeyCode::RIGHT) == eKeyState::DOWN)
		{
			ChangeAnimationClip(BoneAnimator::eCharacterAnimState::STRAFE_RIGHT);
			ChangePlayerState(ePlayerState::STRAFE);
			return true;
		}
		return false;
	}
	const bool IsChangeStrafeLeftState()
	{
		if (InputManager::GetKeyState(eKeyCode::LEFT) == eKeyState::DOWN)
		{
			ChangeAnimationClip(BoneAnimator::eCharacterAnimState::STRAFE_LEFT);
			ChangePlayerState(ePlayerState::STRAFE);
			return true;
		}
		return false;
	}
	const bool IsChangeBackWalkState()
	{
		if (InputManager::GetKeyState(eKeyCode::DOWN) == eKeyState::PRESSED)
		{
			ChangeAnimationClip(BoneAnimator::eCharacterAnimState::BACK_WALK);
			ChangePlayerState(ePlayerState::WALK);
			return true;
		}
		return false;
	}
	const bool IsChangeAttackState()
	{
		if (InputManager::GetKeyState(eKeyCode::Z) == eKeyState::DOWN)
		{
			ChangeAnimationClip(BoneAnimator::eCharacterAnimState::ATTACK_SLASH_1);
			ChangePlayerState(ePlayerState::ATTACK_SLASH_1);
			return true;
		}
		return false;
	}
	const bool IsChangeRollState()
	{
		if (InputManager::GetKeyState(eKeyCode::C) == eKeyState::DOWN)
		{
			ChangeAnimationClip(BoneAnimator::eCharacterAnimState::ROLL);
			ChangePlayerState(ePlayerState::ROLL);
			return true;
		}
		return false;
	}
	const bool IsChangeKickState()
	{
		if (InputManager::GetKeyState(eKeyCode::X) == eKeyState::DOWN)
		{
			ChangeAnimationClip(BoneAnimator::eCharacterAnimState::KICK);
			ChangePlayerState(ePlayerState::KICK);
			return true;
		}
		return false;
	}


	void ChangeIdleStateIfCompletelyPlayCurrentAnimClip()
	{
		if (mpAnimator->IsCurrentAnimClipLastFrame())
		{
			ChangeAnimationClip(BoneAnimator::eCharacterAnimState::IDLE);
			ChangePlayerState(ePlayerState::IDLE);
		}
	}

	void ChangeAnimationClip(const BoneAnimator::eCharacterAnimState eAnimState)
	{
		mpOwnerScript->ChangeAnimationClip(eAnimState);
	}
	void ChangePlayerState(const ePlayerState eState)
	{
		mpOwnerScript->ChangePlayerState(eState);
	}

protected:
	float mWalkSpeed;
	float mTurnRotationSpeedDeg;
	Transform* mpTransform;
	BoneAnimator* mpAnimator;
};


class PlayerIdleState final : public PlayerState
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
		if (IsChangeWalkState())		{ return; }
		if (IsChangeAttackState())		{ return; }
		if (IsChangeBackWalkState())	{ return; }
		if (IsChangeStrafeRightState()) { return; }
		if (IsChangeStrafeLeftState())	{ return; }
		if (IsChangeRollState())		{ return; }
		if (IsChangeKickState())		{ return; }
	}

	void Exit() override
	{
	}
};


class PlayerWalkingState final : public PlayerState
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
		Vector3 moveVector = mpTransform->GetPosition();
		switch (mpOwnerScript->GetCurrAnimState())
		{
		case BoneAnimator::eCharacterAnimState::WALK:
		{
			if (InputManager::GetKeyState(eKeyCode::RIGHT) == eKeyState::PRESSED)
			{
				mpTransform->AccumulateYaw(mTurnRotationSpeedDeg * 2 * Time::DeltaTime());
			}
			if (InputManager::GetKeyState(eKeyCode::LEFT) == eKeyState::PRESSED)
			{
				mpTransform->AccumulateYaw(-mTurnRotationSpeedDeg * 2 * Time::DeltaTime());
			}
			moveVector += -mpTransform->GetForwardRef() * mWalkSpeed * Time::DeltaTime();
			mpTransform->SetPosition(moveVector);
			if (InputManager::GetKeyState(eKeyCode::UP) != eKeyState::PRESSED)
			{
				ChangeAnimationClip(BoneAnimator::eCharacterAnimState::IDLE);
				ChangePlayerState(ePlayerState::IDLE);
				return;
			}
			break;
		}
		case BoneAnimator::eCharacterAnimState::BACK_WALK:
		{
			moveVector += mpTransform->GetForwardRef() * mWalkSpeed * Time::DeltaTime();
			mpTransform->SetPosition(moveVector);
			if (InputManager::GetKeyState(eKeyCode::DOWN) != eKeyState::PRESSED)
			{
				ChangeAnimationClip(BoneAnimator::eCharacterAnimState::IDLE);
				ChangePlayerState(ePlayerState::IDLE);
				return;
			}
			break;
		}
		default:
			break;
		}

		if (IsChangeAttackState())	{ return; }
		if (IsChangeKickState())	{ return; }
	}
	void Exit() override
	{
	}
};

class PlayerStrafeState final : public PlayerState
{
public:
	PlayerStrafeState(PlayerScript* pPlayerScript)
		: PlayerState(pPlayerScript)
	{
	}
	virtual ~PlayerStrafeState() = default;

	void Enter() override
	{
	}

	void Excute() override
	{
		Vector3 moveVector = mpTransform->GetPosition();
		switch (mpOwnerScript->GetCurrAnimState())
		{
		case BoneAnimator::eCharacterAnimState::STRAFE_RIGHT:
		{
			moveVector -= mpTransform->GetRightRef() * mWalkSpeed * Time::DeltaTime();
			break;
		}

		case BoneAnimator::eCharacterAnimState::STRAFE_LEFT:
		{
			moveVector += mpTransform->GetRightRef() * mWalkSpeed * Time::DeltaTime();
			break;
		}
		default:
			break;
		}
		mpTransform->SetPosition(moveVector);
		if (IsChangeIdleState())		{ return; }
		if (IsChangeWalkState())		{ return; }
		if (IsChangeBackWalkState())	{ return; }
		if (IsChangeAttackState())		{ return; }
		if (IsChangeKickState())		{ return; }

	}
	void Exit() override
	{
	}
};

class PlayerRollState final : public PlayerState
{
public:
	PlayerRollState(PlayerScript* pPlayerScript)
		: PlayerState(pPlayerScript)
	{
	}
	virtual ~PlayerRollState() = default;

	void Enter() override
	{
	}

	void Excute() override
	{
		Vector3 moveVector = mpTransform->GetPosition();
		moveVector -= mpTransform->GetForwardRef() * mWalkSpeed * Time::DeltaTime();
		mpTransform->SetPosition(moveVector);
		ChangeIdleStateIfCompletelyPlayCurrentAnimClip();
	}
	void Exit() override
	{
	}
};

class PlayerKickState final : public PlayerState
{
public:
	PlayerKickState(PlayerScript* pPlayerScript)
		: PlayerState(pPlayerScript)
	{
	}
	virtual ~PlayerKickState() = default;

	void Enter() override
	{
	}

	void Excute() override
	{
		ChangeIdleStateIfCompletelyPlayCurrentAnimClip();
	}
	void Exit() override
	{
	}
};

class PlayerAttackState : public PlayerState
{
public:
	PlayerAttackState(PlayerScript* pPlayerScript)
		: PlayerState(pPlayerScript)
		, mbIsZKeyDown(false)
		, mbIsCurrentAttackCilpUpper70Percent(false)
		, mbIsGoToNextAttack(false)
	{
	}
	void Enter() override
	{
		mbIsZKeyDown = false;
		mbIsCurrentAttackCilpUpper70Percent = false;
		mbIsGoToNextAttack = false;
	}
	virtual void Excute() override
	{
		SetCurrentPlayingClipUpper70Percent();
	}
	void Exit() override
	{
	}

protected:
	void SetCurrentPlayingClipUpper70Percent()
	{
		if (mbIsGoToNextAttack)
			{ return; }

		if (mpAnimator->GetCurrentPlayingClipPercentage() >= 0.7f)
			{ mbIsCurrentAttackCilpUpper70Percent = true; }
		else
			{ mbIsCurrentAttackCilpUpper70Percent = false; }

	}
	void SetIsZKeyDown()
	{
		if (mbIsGoToNextAttack) {return;}
		if (InputManager::GetKeyState(eKeyCode::Z) == eKeyState::PRESSED)
		{ 
			mbIsZKeyDown = true; 
			mbIsGoToNextAttack = true;
		}
		else
		{ 
			mbIsZKeyDown = false; 
		}
	}

protected:
	bool mbIsZKeyDown;
	bool mbIsCurrentAttackCilpUpper70Percent;
	bool mbIsGoToNextAttack;
};

class PlayerAttackSlash_1_State final : public PlayerAttackState
{
public:
	PlayerAttackSlash_1_State(PlayerScript* pPlayerScript)
		: PlayerAttackState(pPlayerScript)
	{
	}
	virtual ~PlayerAttackSlash_1_State() = default;

	void Excute() override
	{
		if (mbIsGoToNextAttack)
		{
			if (mpAnimator->IsCurrentAnimClipLastFrame())
			{
				ChangeAnimationClip(BoneAnimator::eCharacterAnimState::ATTACK_SLASH_2);
				ChangePlayerState(ePlayerState::ATTACK_SLASH_2);
			}
		}
		else
		{
			ChangeIdleStateIfCompletelyPlayCurrentAnimClip();
		}

		PlayerAttackState::Excute();
		if (mbIsCurrentAttackCilpUpper70Percent)
		{ 
			SetIsZKeyDown(); 
		}
	}

};

class PlayerAttackSlash_2_State final : public PlayerAttackState
{
public:
	PlayerAttackSlash_2_State(PlayerScript* pPlayerScript)
		: PlayerAttackState(pPlayerScript)
	{
	}
	virtual ~PlayerAttackSlash_2_State() = default;


	void Excute() override
	{
		if (mbIsGoToNextAttack)
		{
			if (mpAnimator->IsCurrentAnimClipLastFrame())
			{
				ChangeAnimationClip(BoneAnimator::eCharacterAnimState::ATTACK_SLASH_3);
				ChangePlayerState(ePlayerState::ATTACK_SLASH_3);
			}
		}
		else
		{
			ChangeIdleStateIfCompletelyPlayCurrentAnimClip();
		}

		PlayerAttackState::Excute();
		if (mbIsCurrentAttackCilpUpper70Percent)
		{
			SetIsZKeyDown();
		}
	}


};

class PlayerAttackSlash_3_State final : public PlayerAttackState
{
public:
	PlayerAttackSlash_3_State(PlayerScript* pPlayerScript)
		: PlayerAttackState(pPlayerScript)
	{
	}
	virtual ~PlayerAttackSlash_3_State() = default;


	void Excute() override
	{
		ChangeIdleStateIfCompletelyPlayCurrentAnimClip();
	}

};

class PlayerHittedState final : public PlayerState
{
public:
	PlayerHittedState(PlayerScript* pPlayerScript)
		: PlayerState(pPlayerScript)
	{
	}
	virtual ~PlayerHittedState() = default;

	void Enter() override
	{
	}
	void Excute() override
	{
		if (mpAnimator->IsCurrentAnimClipLastFrame())
		{
			ChangeAnimationClip(BoneAnimator::eCharacterAnimState::IDLE);
			ChangePlayerState(ePlayerState::IDLE);
		}
	}
	void Exit() override
	{
	}

};

}
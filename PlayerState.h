#pragma once
#include "State.h"
#include "Player.h"

#include "InputManager.h"
#include "Time.h"

#include "Animator.h"
#include "PlayerScript.h"

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
	void ChangeAnimation(const std::string& key)
	{
		mpOwnerScript->ChangeAnimation(key);
	}
	void ChangeState(const ePlayerState eState)
	{
		mpOwnerScript->ChangeState(eState);
	}

	void DodgeAnimationCompletedCallBack()
	{
		mpOwnerScript->ChangeState(ePlayerState::IDLE);
	}

	void HittedAnimationStartCallBack()
	{
		// TODO : HittedAnim Start Preprocess
	}
	void HittedAnimationCompletedCallBack()
	{
		mpOwnerScript->ChangeState(ePlayerState::IDLE);
	}

	const bool IsChangeIdleState()
	{
		if (!InputManager::IsAnyKeyPressed())
		{
			ChangeState(ePlayerState::IDLE);
			return true;
		}
		return false;
	}

	const bool IsChangeRunningState()
	{
		if (InputManager::GetKeyState(eKeyCode::LEFT) == eKeyState::PRESSED || 
			InputManager::GetKeyState(eKeyCode::RIGHT) == eKeyState::PRESSED)
		{
			ChangeState(ePlayerState::RUNNING);
			return true;
		}
		return false;
	}

	const bool IsChangeRollingState()
	{
		if (InputManager::GetKeyState(eKeyCode::C) == eKeyState::DOWN)
		{
			ChangeState(ePlayerState::ROLLING);
			return true;
		}
		return false;
	}

	const bool IsChangeDashState()
	{
		if (InputManager::GetKeyState(eKeyCode::V) == eKeyState::DOWN)
		{
			ChangeState(ePlayerState::DASH);
			return true;
		}
		return false;
	}

	const bool IsChangeComboAttackState()
	{
		if (InputManager::GetKeyState(eKeyCode::Z) == eKeyState::DOWN)
		{
			ChangeState(ePlayerState::ATTACK_1);
			return true;
		}
		return false;
	}
	
	const bool IsChangePushAttackState()
	{
		if (InputManager::GetKeyState(eKeyCode::X) == eKeyState::DOWN)
		{
			ChangeState(ePlayerState::ATTACK_PUSH);
			return true;
		}
		return false;
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
		ChangeAnimation(Player::IDLE_ANIM_KEY);
	}

	void Excute() override
	{
		if (IsChangeRunningState())	return;
		if (IsChangeComboAttackState()) return;
		if (IsChangePushAttackState()) return;
		if (IsChangeRollingState()) return;
		if (IsChangeDashState()) return;
	}

	void Exit() override
	{

	}

};


class PlayerRunningState : public PlayerState
{
public:
	PlayerRunningState(PlayerScript* pPlayerScript)
		: PlayerState(pPlayerScript)
	{
	}
	virtual ~PlayerRunningState() = default;

	void Enter() override
	{
		ChangeAnimation(Player::RUN_ANIM_KEY);
	}

	void Excute() override
	{
		if (IsChangeIdleState()) return;
		if (IsChangeComboAttackState()) return;
		if (IsChangePushAttackState()) return;
		if (IsChangeRollingState()) return;
		if (IsChangeDashState()) return;
	}

	void Exit() override
	{
	}
};


class PlayerRollingState : public PlayerState
{
public:
	PlayerRollingState(PlayerScript* pPlayerScript)
		: PlayerState(pPlayerScript)
	{
	}
	virtual ~PlayerRollingState() = default;

	void Enter() override
	{
		ChangeAnimation(Player::PLAYER_ROLLING_KEY);
	}

	void Excute() override
	{
	}

	void Exit() override
	{
	}
};

class PlayerDashState : public PlayerState
{
public:
	PlayerDashState(PlayerScript* pPlayerScript)
		: PlayerState(pPlayerScript)
	{
	}
	virtual ~PlayerDashState() = default;

	void Enter() override
	{
		ChangeAnimation(Player::PLAYER_DASH_KEY);
	}

	void Excute() override
	{
	}

	void Exit() override
	{
	}
};

class PlayerHittedState : public PlayerState
{
public:
	PlayerHittedState(PlayerScript* pPlayerScript)
		: PlayerState(pPlayerScript)
	{
	}
	virtual ~PlayerHittedState() = default;

	void Enter() override
	{
		ChangeAnimation(Player::PLAYER_HITTED_KEY);
	}

	void Excute() override
	{
	}

	void Exit() override
	{
	}
};

#pragma region ATTACK_PART
enum class ePlayerAttackType
{
	ATTACK_1,
	ATTACK_2,
	ATTACK_3,
	ATTACK_PUSH,
	COUNT
};

class PlayerAttackState : public PlayerState
{
public:
	PlayerAttackState(PlayerScript* pPlayerScript, ePlayerAttackType eType)
		: PlayerState(pPlayerScript)
		, mePlayerAttackType(eType)
	{
	}
	virtual ~PlayerAttackState() = default;
	void Excute() override
	{
		CheckContinueAttack();
	}
	void Exit() override
	{
		ResetContinueAttackFlag();
	}


	void AttckAnimationCompletedCallBack()
	{
		if (mbIsContinueAttackFlag)
		{
			switch (mePlayerAttackType)
			{
			case jh::fsm::ePlayerAttackType::ATTACK_1:
				ChangeState(ePlayerState::ATTACK_2);
				break;
			case jh::fsm::ePlayerAttackType::ATTACK_2:
				ChangeState(ePlayerState::ATTACK_3);
				break;
			case jh::fsm::ePlayerAttackType::ATTACK_3:
				ChangeState(ePlayerState::IDLE);
				break;
			default:
				assert(false);
				break;
			}
		}
		else
		{
			ChangeState(ePlayerState::IDLE);
		}
	}

	void CheckContinueAttack()
	{
		if (InputManager::GetKeyState(eKeyCode::Z) == eKeyState::DOWN)
		{
			mbIsContinueAttackFlag = true;
		}
	}

	void ResetContinueAttackFlag()
	{
		mbIsContinueAttackFlag = false;
	}

protected:
	ePlayerAttackType mePlayerAttackType;
	bool mbIsContinueAttackFlag = false;
};

class PlayerAttackOneState : public PlayerAttackState
{
public:
	PlayerAttackOneState(PlayerScript* pPlayerScript)
		: PlayerAttackState(pPlayerScript, ePlayerAttackType::ATTACK_1)
	{
	}
	virtual ~PlayerAttackOneState() = default;

	void Enter() override
	{
		ChangeAnimation(Player::ATTACK_1_ANIM_KEY);
	}
};

class PlayerAttackTwoState : public PlayerAttackState
{
public:
	PlayerAttackTwoState(PlayerScript* pPlayerScript)
		: PlayerAttackState(pPlayerScript, ePlayerAttackType::ATTACK_2)
	{
	}
	virtual ~PlayerAttackTwoState() = default;

	void Enter() override
	{
		ChangeAnimation(Player::ATTACK_2_ANIM_KEY);
	}
};

class PlayerAttackThreeState : public PlayerAttackState
{
public:
	PlayerAttackThreeState(PlayerScript* pPlayerScript)
		: PlayerAttackState(pPlayerScript, ePlayerAttackType::ATTACK_3)
	{
	}
	virtual ~PlayerAttackThreeState() = default;

	void Enter() override
	{
		ChangeAnimation(Player::ATTACK_3_ANIM_KEY);
	}
};
class PlayerAtttackPushState : public PlayerAttackState
{
public:
	PlayerAtttackPushState(PlayerScript* pPlayerScript)
		: PlayerAttackState(pPlayerScript, ePlayerAttackType::ATTACK_PUSH)
	{
	}
	virtual ~PlayerAtttackPushState() = default;

	void Enter() override
	{
		ChangeAnimation(Player::ATTACK_PUSH_ANIM_KEY);
	}
	void Excute() override
	{
		// NONE
	}
};
#pragma endregion

}
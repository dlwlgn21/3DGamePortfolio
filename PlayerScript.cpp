#include "PlayerScript.h"
#include "BoneAnimator.h"
#include "GameObject.h"
#include "Transform.h"

#include "PlayerState.h"
#include "AnimationDataManager.h"
#include "InputManager.h"
#include "Time.h"

namespace jh
{

PlayerScript::PlayerScript()
	: Script()
	, mFSM()
	, mpStates()
	, mpAnimator(nullptr)
	, meAnimState(BoneAnimator::eCharacterAnimState::IDLE)
	, mePlayerState(ePlayerState::IDLE)
	, mWalkSpeed(1.0f)
	, mTurnRotationSpeedDeg(180.0f)
{
	for (UINT i = 0; i < static_cast<UINT>(ePlayerState::COUNT); ++i)
	{
		mpStates[i] = nullptr;
	}
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
	mpStates[static_cast<UINT>(ePlayerState::IDLE)] = new jh::fsm::PlayerIdleState(this);
	mpStates[static_cast<UINT>(ePlayerState::WALK)] = new jh::fsm::PlayerWalkingState(this);
	mpStates[static_cast<UINT>(ePlayerState::STRAFE)] = new jh::fsm::PlayerStrafeState(this);
	mpStates[static_cast<UINT>(ePlayerState::ROLL)] = new jh::fsm::PlayerRollState(this);
	mpStates[static_cast<UINT>(ePlayerState::KICK)] = new jh::fsm::PlayerKickState(this);
	mpStates[static_cast<UINT>(ePlayerState::ATTACK_SLASH_1)] = new jh::fsm::PlayerAttackSlash_1_State(this);
	mpStates[static_cast<UINT>(ePlayerState::ATTACK_SLASH_2)] = new jh::fsm::PlayerAttackSlash_2_State(this);
	mpStates[static_cast<UINT>(ePlayerState::ATTACK_SLASH_3)] = new jh::fsm::PlayerAttackSlash_3_State(this);
	mFSM.Init(*mpStates[static_cast<UINT>(ePlayerState::IDLE)]);
}
void PlayerScript::Update()
{
	mFSM.Excute();
}

void PlayerScript::ChangeAnimationClip(const BoneAnimator::eCharacterAnimState eAnimState)
{
	auto& clipNames = AnimationDataManager::GetInstance().GetAnimationCilpKeys(eAnimClipKeyContainerType::PLAYER);
	assert(clipNames.size() != 0);
	meAnimState = eAnimState;
	mpAnimator->ChangeCurrentAnimationClip(&clipNames[static_cast<UINT>(eAnimState)]);
}

void PlayerScript::ChangePlayerState(const ePlayerState eState)
{
	assert(eState != mePlayerState);
	mePlayerState = eState;
	mFSM.ChangeState(*mpStates[static_cast<UINT>(eState)]);
}

}
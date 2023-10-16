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
	, mpAnimator(nullptr)
	, meAnimState(BoneAnimator::eCharacterAnimState::IDLE)
	, mePlayerState(ePlayerState::IDLE)
{
	mpStates[static_cast<UINT>(ePlayerState::IDLE)] = new jh::fsm::PlayerIdleState(this);
	mpStates[static_cast<UINT>(ePlayerState::WALK)] = new jh::fsm::PlayerWalkingState(this);
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

}
void PlayerScript::Update()
{
	mFSM.Excute();
}

void PlayerScript::ChangeAnimationClip(const BoneAnimator::eCharacterAnimState eAnimState)
{
	auto& clipNames = AnimationDataManager::GetInstance().GetAnimationCilpKeys(eAnimClipKeyContainerType::PLAYER);
	assert(clipNames.size() != 0);
	const UINT INDEX = static_cast<UINT>(eAnimState);
	meAnimState = eAnimState;
	mpAnimator->ChangeCurrentAnimationClip(&clipNames[INDEX]);
}

void PlayerScript::ChangePlayerState(const ePlayerState eState)
{
	assert(eState != mePlayerState);
	mePlayerState = eState;
	mFSM.ChangeState(*mpStates[static_cast<UINT>(eState)]);
}

}
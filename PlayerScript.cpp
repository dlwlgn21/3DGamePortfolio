#include "PlayerScript.h"
#include "BoneAnimator.h"
#include "GameObject.h"
#include "Transform.h"

#include "AnimationDataManager.h"
#include "InputManager.h"
#include "Time.h"

namespace jh
{

PlayerScript::PlayerScript()
	: Script()
	, mpAnimator(nullptr)
	, meAnimState(BoneAnimator::eCharacterAnimState::IDLE)
{
}

void PlayerScript::Initialize()
{

}
void PlayerScript::Update()
{
	UINT clipIndex = static_cast<UINT>(meAnimState);
	switch (meAnimState)
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
		if (mpAnimator->IsCurrentAnimClipLastFrame())
		{
			ChangeAnimationClip(BoneAnimator::eCharacterAnimState::WALK_FORWARD);
		}
		break;
	}
	case BoneAnimator::eCharacterAnimState::WALK_FORWARD:
	{
		auto& transform = GetOwner().GetTransform();
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
		if (mpAnimator->IsCurrentAnimClipLastFrame())
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
		if (mpAnimator->IsCurrentAnimClipLastFrame())
		{
			ChangeAnimationClip(BoneAnimator::eCharacterAnimState::IDLE);
		}
		break;
	}
	default:
		assert(false);
		break;
	}
}
void PlayerScript::ChangeAnimationClip(const BoneAnimator::eCharacterAnimState eAnimState)
{
	auto& clipNames = AnimationDataManager::GetInstance().GetAnimationCilpKeys(eAnimClipKeyContainerType::PLAYER);
	assert(clipNames.size() != 0);
	const UINT INDEX = static_cast<UINT>(eAnimState);
	meAnimState = eAnimState;
	mpAnimator->ChangeCurrentAnimationClip(&clipNames[INDEX]);
}

}
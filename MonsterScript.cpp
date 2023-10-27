#include "MonsterScript.h"
#include "BoneAnimator.h"
#include "GameObject.h"
#include "Transform.h"

#include "MonsterState.h"
#include "AnimationDataManager.h"
#include "InputManager.h"
#include "Time.h"

namespace jh
{
	MonsterScript::MonsterScript()
		: Script()
		, mFSM()
		, mpStates()
		, mpAnimator(nullptr)
		, meAnimState(BoneAnimator::eMonsterAnimState::IDLE)
		, meMonsterState(eMonsterState::IDLE)
		, mWalkSpeed(0.6f)
		, mTurnRotationSpeedDeg(180.0f)
	{
		for (UINT i = 0; i < static_cast<UINT>(eMonsterState::COUNT); ++i)
		{
			mpStates[i] = nullptr;
		}

	}

	MonsterScript::~MonsterScript()
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

	void MonsterScript::Initialize()
	{
		mpStates[static_cast<UINT>(eMonsterState::IDLE)] = new jh::fsm::MonsterIdleState(this);
		mFSM.Init(*mpStates[static_cast<UINT>(eMonsterState::IDLE)]);
	}
	void MonsterScript::Update()
	{
		mFSM.Excute();
	}

	void MonsterScript::ChangeAnimationClip(const BoneAnimator::eMonsterAnimState eAnimState)
	{
		auto& clipNames = AnimationDataManager::GetInstance().GetAnimationCilpKeys(eAnimClipKeyContainerType::MONSTER);
		assert(clipNames.size() != 0);
		meAnimState = eAnimState;
		mpAnimator->ChangeCurrentAnimationClip(&clipNames[static_cast<UINT>(eAnimState)]);
	}

	void MonsterScript::ChangeMonsterState(const eMonsterState eState)
	{
		assert(eState != meMonsterState);
		meMonsterState = eState;
		mFSM.ChangeState(*mpStates[static_cast<UINT>(eState)]);
	}

}
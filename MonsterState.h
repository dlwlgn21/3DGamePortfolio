#pragma once
#include "State.h"

#include "Time.h"

#include "GameObject.h"
#include "Transform.h"
#include "MonsterScript.h"
#include "BoneAnimator.h"
#include "Collider3D.h"

#include "PlayerManager.h"
#include "PlayerScript.h"

#include "DebugHelper.h"

using namespace jh::enums;
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace jh::fsm
{

class MonsterState : public State<MonsterScript>
{
public:
	MonsterState(MonsterScript* pMonsterScript)
		: State(pMonsterScript)
		, mWalkSpeed(pMonsterScript->GetWalkSpeed())
		, mTurnRotationSpeedDeg(pMonsterScript->GetTurnRoationSpeedDeg())
		, mpTransform(&pMonsterScript->GetOwner().GetTransform())
		, mpAnimator(&pMonsterScript->GetAnimator())
		, mpCollider(&static_cast<Collider3D&>(pMonsterScript->GetOwner().GetComponent(jh::enums::eComponentType::COLLIDER_3D)))
		, mpPlayerScript(&PlayerManager::GetInstance().GetPlayerScript())
		, mpPlayerCollider(&PlayerManager::GetInstance().GetPlayerCollider())
		, mpPlayerTransform(&PlayerManager::GetInstance().GetPlayerTramsform())
	
	{
	}
	virtual ~MonsterState() = default;
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
	void ChangeAnimationClip(const BoneAnimator::eMonsterAnimState eAnimState)
	{
		mpOwnerScript->ChangeAnimationClip(eAnimState);
	}
	void ChangeMonsterState(const eMonsterState eState)
	{
		mpOwnerScript->ChangeMonsterState(eState);
	}

protected:
	float mWalkSpeed;
	float mTurnRotationSpeedDeg;
	Transform* mpTransform;
	BoneAnimator* mpAnimator;
	Collider3D* mpCollider;
	PlayerScript* mpPlayerScript;
	Collider3D* mpPlayerCollider;
	Transform* mpPlayerTransform;
};

class MonsterIdleState final : public MonsterState
{
public:
	MonsterIdleState(MonsterScript* pMonsterScript)
		: MonsterState(pMonsterScript)
	{
	}
	virtual ~MonsterIdleState() = default;

	void Enter() override
	{
	}
	void Excute() override
	{
		static bool bIsFirstCollision = true;
		BoundingSphere& agroSphere = mpCollider->GetSphere(eBoundingSphereType::AGRO_SPHERE);
		BoundingBox& playerHitBox = mpPlayerCollider->GetHitBox();
		if (agroSphere.Intersects(playerHitBox))
		{
			if (bIsFirstCollision)
			{
				bIsFirstCollision = false;
				return;
			}

			ChangeAnimationClip(BoneAnimator::eMonsterAnimState::WALK);
			ChangeMonsterState(eMonsterState::TRACING);
		}
	}
	void Exit() override
	{
	}
};


class MonsterTracingState final : public MonsterState
{
public:
	MonsterTracingState(MonsterScript* pMonsterScript)
		: MonsterState(pMonsterScript)
	{
	}
	virtual ~MonsterTracingState() = default;

	void Enter() override
	{
	}
	void Excute() override
	{
		Vector3 toPlayerDir = mpPlayerTransform->GetPosition() - mpTransform->GetPosition();
		BoundingSphere& attackSphere = mpCollider->GetSphere(eBoundingSphereType::ATTACK_SPHERE);
		BoundingBox& playerHitBox = mpPlayerCollider->GetHitBox();
		if (attackSphere.Intersects(playerHitBox))
		{
			ChangeAnimationClip(BoneAnimator::eMonsterAnimState::SLASH_ATTACK);
			ChangeMonsterState(eMonsterState::ATTACKING);
		}
		toPlayerDir.Normalize();
		auto moveVector = mpTransform->GetPosition();
		moveVector += toPlayerDir * mpOwnerScript->GetWalkSpeed() * Time::DeltaTime();
		mpTransform->SetPosition(moveVector);
		Quaternion q = Quaternion::FromToRotation(-mpTransform->GetForwardRef(), toPlayerDir);
		Vector3 euler = q.ToEuler();
		float angle = DirectX::XMConvertToDegrees(euler.y);
		mpTransform->AccumulateYaw(angle * 5.0f * Time::DeltaTime());
	}
	void Exit() override
	{
	}
};


class MonsterAttackingState final : public MonsterState
{
public:
	MonsterAttackingState(MonsterScript* pMonsterScript)
		: MonsterState(pMonsterScript)
	{
	}
	virtual ~MonsterAttackingState() = default;

	void Enter() override
	{
	}
	void Excute() override
	{
		
	}
	void Exit() override
	{
	}
};

}
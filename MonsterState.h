#pragma once
#include "State.h"

#include "GameObject.h"
#include "Transform.h"
#include "MonsterScript.h"
#include "BoneAnimator.h"
#include "BoxCollider3D.h"

#include "PlayerManager.h"
#include "PlayerScript.h"

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
		, mpBoxCollider(&static_cast<BoxCollider3D&>(pMonsterScript->GetOwner().GetComponent(jh::enums::eComponentType::BOX_COLLIDER_3D)))
		, mpPlayerScript(&PlayerManager::GetInstance().GetPlayerScript())
		, mpPlayerBoxCollider(&PlayerManager::GetInstance().GetPlayerBoxCollider())
	
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
	float mWalkSpeed;
	float mTurnRotationSpeedDeg;
	Transform* mpTransform;
	BoneAnimator* mpAnimator;
	BoxCollider3D* mpBoxCollider;
	PlayerScript* mpPlayerScript;
	BoxCollider3D* mpPlayerBoxCollider;
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
		BoundingBox& agroBox = mpBoxCollider->GetBoundingBox(eBoundingBoxType::AGRO_BOX);
		BoundingBox& playerHitBox = mpPlayerBoxCollider->GetBoundingBox(eBoundingBoxType::HIT_BOX);
		if (agroBox.Intersects(playerHitBox))
		{
			std::cout << playerHitBox.Center.x << ", " << playerHitBox.Center.y << playerHitBox.Center.z << std::endl;
		}
		
	}
	void Exit() override
	{
	}
};

}
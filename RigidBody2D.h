#pragma once
#include "Component.h"

class b2Body;

namespace jh
{
enum class eRigidBodyType
{
	STATIC = 0,
	KINEMATICS = 1,
	DYNAMIC = 2,
	COUNT = 3
};

class RigidBody2D : public Component
{
public:
	RigidBody2D();
	virtual ~RigidBody2D() = default;
	virtual void Initialize();
	virtual void Update();
	virtual void FixedUpdate();

	void SetRBType(const eRigidBodyType eType);
	eRigidBodyType GetRBType() const { return meRBType; }
	bool GetIsFreezRotation() const { return mbIsFrezzingRoation; }

	const b2Body& GetBody();
	b2Body* GetB2BodyOrNull();
	void SetB2Body(b2Body* pB2Body);

private:
	eRigidBodyType meRBType = eRigidBodyType::COUNT;
	bool mbIsFrezzingRoation = true;
	b2Body* mpB2Body = nullptr;
};

}
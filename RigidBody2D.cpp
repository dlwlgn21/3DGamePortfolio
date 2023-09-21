#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include "RigidBody2D.h"
#include "GameObject.h"
#include "Transform.h"
#include "Box2DPhysicsManager.h"

using namespace jh::enums;

namespace jh
{

RigidBody2D::RigidBody2D()
	: Component(eComponentType::RIGID_BODY_2D)
{
}

void RigidBody2D::Initialize()
{
}

void RigidBody2D::Update()
{
}

void RigidBody2D::FixedUpdate()
{
	assert(mpB2Body != nullptr);
}

void RigidBody2D::SetRBType(const eRigidBodyType eType)
{
	meRBType = eType;
	b2BodyDef bodyDef;
	const auto& transform = GetOwner().GetTransform();
	const auto& pos = transform.GetOnlyXYPosition();
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.angle = DirectX::XMConvertToRadians(transform.GetRotation().z);

	switch (meRBType)
	{
	case eRigidBodyType::STATIC:
	{
		bodyDef.type = b2BodyType::b2_staticBody;
		break;
	}
	case eRigidBodyType::KINEMATICS:
	{
		bodyDef.type = b2BodyType::b2_kinematicBody;
		break;
	}
	case eRigidBodyType::DYNAMIC:
	{
		bodyDef.type = b2BodyType::b2_dynamicBody;
		break;
	}
	default:
		assert(false);
		break;
	}
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(GetOwnerOrNull());
	mpB2Body = Box2DManager::GetInstance().GetB2World().CreateBody(&bodyDef);
}

const b2Body& RigidBody2D::GetBody()
{
	assert(mpB2Body != nullptr);
	return *mpB2Body;
}

b2Body* RigidBody2D::GetB2BodyOrNull()
{
	return mpB2Body;
}

void RigidBody2D::SetB2Body(b2Body* pB2Body)
{
	assert(pB2Body != nullptr && mpB2Body == nullptr);
	mpB2Body = pB2Body;
}

}

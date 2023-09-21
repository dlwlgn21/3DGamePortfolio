#include "MovingScript.h"

#include "InputManager.h"
#include "Time.h"

#include "GameObject.h"
#include "Transform.h"
#include "RigidBody2D.h"

using namespace DirectX::SimpleMath;
using namespace jh::enums;

namespace jh
{
	MovingScript::MovingScript()
		: Script()
		, mMovementSpeed(2.0f)
		, meMovingState(eMovingState::RIGHT)
	{
	}

	void MovingScript::Initialize()
	{
		mpB2Body = static_cast<RigidBody2D&>(GetOwner().GetComponent(eComponentType::RIGID_BODY_2D)).GetB2BodyOrNull();
		assert(mpB2Body != nullptr);
	}

	void MovingScript::Update()
	{
		b2Vec2 b2LinearVelocity = mpB2Body->GetLinearVelocity();
		float desiredVeocity = 0.0f;
		if (InputManager::GetKeyState(eKeyCode::LEFT) == eKeyState::PRESSED)
		{
			meMovingState = eMovingState::LEFT;
			desiredVeocity = -mMovementSpeed;
		}
		else if (InputManager::GetKeyState(eKeyCode::RIGHT) == eKeyState::PRESSED)
		{
			meMovingState = eMovingState::RIGHT;
			desiredVeocity = +mMovementSpeed;
		}
		else
		{
			desiredVeocity = 0.0f;
		}
		float velocityChange = desiredVeocity - b2LinearVelocity.x;
		float impulse = mpB2Body->GetMass() * velocityChange;
		mpB2Body->ApplyLinearImpulseToCenter(b2Vec2(impulse, 0.0f), true);

	}
}
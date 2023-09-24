#include "MovingScript.h"

#include "InputManager.h"
#include "Time.h"

#include "GameObject.h"
#include "Transform.h"

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
	}

	void MovingScript::Update()
	{

	}
}
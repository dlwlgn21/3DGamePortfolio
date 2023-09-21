#include "CameraScript.h"
#include "GameObject.h"
#include "Transform.h"
#include "InputManager.h"
#include "Time.h"

using namespace DirectX::SimpleMath;

namespace jh
{


CameraScript::CameraScript()
	: Script()
	, mMovementSpeed(3.0f)
{
}

void CameraScript::Update()
{
	Transform& transform = GetOwner().GetTransform();
	Vector3 pos = transform.GetPosition();

	if (InputManager::GetKeyState(eKeyCode::D) == eKeyState::PRESSED)
	{
		pos += mMovementSpeed * transform.GetRight() * Time::DeltaTime();
	}
	else if (InputManager::GetKeyState(eKeyCode::A) == eKeyState::PRESSED)
	{
		pos += mMovementSpeed * -transform.GetRight() * Time::DeltaTime();
	}
	else if (InputManager::GetKeyState(eKeyCode::W) == eKeyState::PRESSED)
	{
		pos += mMovementSpeed * transform.GetForward() * Time::DeltaTime();
	}
	else if (InputManager::GetKeyState(eKeyCode::S) == eKeyState::PRESSED)
	{
		pos += mMovementSpeed * -transform.GetForward() * Time::DeltaTime();
	}
	else if (InputManager::GetKeyState(eKeyCode::Q) == eKeyState::PRESSED)
	{
		pos += mMovementSpeed * transform.GetUp() * Time::DeltaTime();
	}
	else if (InputManager::GetKeyState(eKeyCode::E) == eKeyState::PRESSED)
	{
		pos += mMovementSpeed * -transform.GetUp() * Time::DeltaTime();
	}
	transform.SetPosition(pos);
}


}
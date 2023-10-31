#include "CameraScript.h"

#include "InputManager.h"
#include "Time.h"

#include "GameObject.h"
#include "Transform.h"
#include "PlayerManager.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

namespace jh
{

CameraScript::CameraScript()
	: Script()
	, mMovementSpeed(5.0f)
	, mpTransform(nullptr)
	, mbIsPressedSpaceBar(false)
{
}

void CameraScript::Initialize()
{
	mpTransform = &GetOwner().GetTransform();
}

void CameraScript::Update()
{
	Vector3 pos = mpTransform->GetPosition();
	auto& playerTr = PlayerManager::GetInstance().GetPlayerTramsform();
	pos = playerTr.GetPosition();
	//pos = Vector3::Transform(pos, Matrix::CreateRotationY(DirectX::XMConvertToRadians(-playerTr.GetYawDeg())));
	pos.y += 0.7f;
	pos.z += 1.0f;
	mpTransform->SetPosition(pos);
	if (InputManager::GetKeyState(eKeyCode::SPACE) == eKeyState::PRESSED)
	{
		mbIsPressedSpaceBar = true;
		return;
	}
	if (InputManager::GetKeyState(eKeyCode::SPACE) == eKeyState::UP)
	{
		mbIsPressedSpaceBar = false;
		return;
	}

	//if (mbIsPressedSpaceBar)
	//{
	//	return;
	//}

	//if (InputManager::GetKeyState(eKeyCode::W) == eKeyState::PRESSED)
	//{
	//	pos += fowardDir * mMovementSpeed * -Time::DeltaTime();
	//}
	//else if (InputManager::GetKeyState(eKeyCode::S) == eKeyState::PRESSED)
	//{
	//	pos += fowardDir * mMovementSpeed * Time::DeltaTime();
	//}

	//if (InputManager::GetKeyState(eKeyCode::D) == eKeyState::PRESSED)
	//{
	//	pos += rightDir * mMovementSpeed * Time::DeltaTime();
	//}
	//else if (InputManager::GetKeyState(eKeyCode::A) == eKeyState::PRESSED)
	//{
	//	pos += rightDir * mMovementSpeed * -Time::DeltaTime();
	//}
	//mpTransform->SetPosition(pos);
}

void CameraScript::MouseUpdate(const float mouseNdcX, const float mouseNdcY)
{
	if (mbIsPressedSpaceBar)
	{
		return;
	}
	 
	Rotation& rot = mpTransform->GetRotationRef();
	Vector3& viewDir = mpTransform->GetForwardRef();
	Vector3& rightDir = mpTransform->GetRightRef();
	rot.YawDeg = mouseNdcX * XMConvertToDegrees(XM_2PI); // 좌우 360도
	rot.PitchDeg = mouseNdcY * XMConvertToDegrees(XM_PIDIV2); // 위 아래 90도
	viewDir = Vector3::Transform(Transform::S_FORWARD, Matrix::CreateRotationY(XMConvertToRadians(rot.YawDeg)));
	rightDir = Transform::S_UP.Cross(viewDir);

}

}
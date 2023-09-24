#include "CameraManager.h"
#include "Camera.h"
#include "Transform.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;

namespace jh
{
	Vector3& CameraManager::GetRotation()
	{
		return mpCamera->GetOwner().GetTransform().GetRotationRef();
	}
}
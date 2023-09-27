#include "CameraManager.h"

#include "Transform.h"
#include "GameObject.h"
#include "Camera.h"
#include "CameraScript.h"

using namespace DirectX::SimpleMath;
using namespace jh::enums;

namespace jh
{

void CameraManager::SetCamera(Camera& camera) 
{ 
	assert(mpCamera == nullptr); 
	mpCamera = &camera; 
	mpScript = static_cast<CameraScript*>(&mpCamera->GetOwner().GetScript(eScriptType::MOVING));
}

void CameraManager::OnMouseMove(float mouseNdcX, float mouseNdcY)
{
	mpScript->MouseUpdate(mouseNdcX, mouseNdcY);
}

}
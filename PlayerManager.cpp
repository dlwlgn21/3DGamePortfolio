#include "PlayerManager.h"

#include "GameObject.h"
#include "PlayerScript.h"
#include "Transform.h"
#include "BoxCollider3D.h"

namespace jh
{
	PlayerScript& PlayerManager::GetPlayerScript()
	{
		assert(mpPlayerScript != nullptr);
		return *mpPlayerScript;
	}
	Transform& PlayerManager::GetPlayerTramsform()
	{
		assert(mpPlayerScript != nullptr);
		return mpPlayerScript->GetOwner().GetTransform();
	}
	BoxCollider3D& PlayerManager::GetPlayerBoxCollider()
	{
		assert(mpPlayerScript != nullptr);
		return static_cast<BoxCollider3D&>(mpPlayerScript->GetOwner().GetComponent(jh::enums::eComponentType::BOX_COLLIDER_3D));
	}
}
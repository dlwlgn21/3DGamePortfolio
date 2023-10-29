#include "PlayerManager.h"

#include "GameObject.h"
#include "PlayerScript.h"
#include "Transform.h"
#include "Collider3D.h"

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
	Collider3D& PlayerManager::GetPlayerCollider()
	{
		assert(mpPlayerScript != nullptr);
		return static_cast<Collider3D&>(mpPlayerScript->GetOwner().GetComponent(jh::enums::eComponentType::COLLIDER_3D));
	}
}
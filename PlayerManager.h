#pragma once
#include "Common.h"

namespace jh
{

class PlayerScript;
class Transform;
class BoxCollider3D;
class PlayerManager final
{
public:
	static PlayerManager& GetInstance()
	{
		static PlayerManager instance;
		return instance;
	}

	PlayerManager(const PlayerManager& other) = delete;
	PlayerManager& operator=(const PlayerManager& other) = delete;

	void SetPlayerScript(PlayerScript& script) { mpPlayerScript = &script; }

	PlayerScript& GetPlayerScript();
	Transform& GetPlayerTramsform();
	BoxCollider3D& GetPlayerBoxCollider();

private:
	PlayerManager() = default;
	~PlayerManager() = default;

private:
	PlayerScript* mpPlayerScript;
};

}


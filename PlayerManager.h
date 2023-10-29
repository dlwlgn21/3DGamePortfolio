#pragma once
#include "Common.h"

namespace jh
{

class PlayerScript;
class Transform;
class Collider3D;
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
	Collider3D& GetPlayerCollider();

private:
	PlayerManager() = default;
	~PlayerManager() = default;

private:
	PlayerScript* mpPlayerScript;
};

}


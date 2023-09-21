#pragma once
#include "Entity.h"
#include <directxtk/SimpleMath.h>

namespace jh
{


class GameObject;
class Component : public Entity
{
public:
	Component(jh::enums::eComponentType eType);
	virtual ~Component() = default;

	virtual void Initialize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	const jh::enums::eComponentType GetType() const { return meType; }
	void SetOwner(GameObject* pOwner) { assert(pOwner != nullptr); mpOwner = pOwner; }
	GameObject& GetOwner() const { assert(mpOwner != nullptr); return *mpOwner; }
	GameObject* GetOwnerOrNull() { assert(mpOwner != nullptr); return mpOwner; }

protected:
	GameObject* mpOwner;
	const jh::enums::eComponentType meType;
};


}



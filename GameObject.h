#pragma once
#include "Entity.h"
#include "Component.h"
#include "Script.h"
#include "Transform.h"

namespace jh
{


class GameObject : public Entity
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Initialize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	Component& AddComponent(const jh::enums::eComponentType eType, std::unique_ptr<Component> uniquePtr);

	Component& GetComponent(const jh::enums::eComponentType eType);
	Component* GetComponentOrNull(const jh::enums::eComponentType eType);

	Transform& GetTransform() { assert(mpTransform != nullptr); return *mpTransform; }

	void AddScript(const jh::enums::eScriptType eType, std::unique_ptr<Script> uniquePtr);
	Script& GetScript(const jh::enums::eScriptType eType);
	Script* GetScriptOrNull(const jh::enums::eScriptType eType);

protected:
	std::vector<std::unique_ptr<Component>>			mspComponents;
	std::vector<std::unique_ptr<Script>>			mspScripts;
	Transform*										mpTransform;
};



}


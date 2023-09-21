#include "GameObject.h"

using namespace jh::enums;

namespace jh
{


GameObject::GameObject()
	: Entity()
	, mspComponents()
	, mpTransform()
{
	mspComponents.reserve(static_cast<UINT>(eComponentType::COUNT));
	mspComponents.resize(mspComponents.capacity());
	mspComponents[static_cast<UINT>(eComponentType::TRANSFORM)] = std::make_unique<Transform>();
	mpTransform = static_cast<Transform*>(mspComponents[static_cast<UINT>(eComponentType::TRANSFORM)].get());
	mpTransform->SetOwner(this);

	mspScripts.reserve(static_cast<UINT>(eScriptType::COUNT));
	mspScripts.resize(mspScripts.capacity());
}

GameObject::~GameObject()
{
	mspComponents.clear();
	mspScripts.clear();
}
void GameObject::Initialize()
{
	for (UINT i = 0; i < static_cast<UINT>(eComponentType::COUNT); ++i)
	{
		if (mspComponents[i] == nullptr)
		{
			continue;
		}
		mspComponents[i]->Initialize();
	}
	for (UINT i = 0; i < static_cast<UINT>(eScriptType::COUNT); ++i)
	{
		if (mspScripts[i] == nullptr)
		{
			continue;
		}
		mspScripts[i]->Initialize();
	}
}
void GameObject::Update()
{
	for (UINT i = 0; i < static_cast<UINT>(eComponentType::COUNT); ++i)
	{
		if (mspComponents[i] == nullptr)
		{
			continue;
		}
		mspComponents[i]->Update();
	}
	for (UINT i = 0; i < static_cast<UINT>(eScriptType::COUNT); ++i)
	{
		if (mspScripts[i] == nullptr)
		{
			continue;
		}
		mspScripts[i]->Update();
	}
}
void GameObject::FixedUpdate()
{
	for (UINT i = 0; i < static_cast<UINT>(eComponentType::COUNT); ++i)
	{
		if (mspComponents[i] == nullptr)
		{
			continue;
		}
		mspComponents[i]->FixedUpdate();
	}
	for (UINT i = 0; i < static_cast<UINT>(eScriptType::COUNT); ++i)
	{
		if (mspScripts[i] == nullptr)
		{
			continue;
		}
		mspScripts[i]->FixedUpdate();
	}
}
void GameObject::Render()
{
	if (mspComponents[static_cast<UINT>(eComponentType::RENDERER)] != nullptr)
	{
		mspComponents[static_cast<UINT>(eComponentType::RENDERER)]->Render();
	}
}

Component& GameObject::AddComponent(const eComponentType eType, std::unique_ptr<Component> uniquePtr)
{
	assert(mspComponents[static_cast<UINT>(eType)] == nullptr);
	mspComponents[static_cast<UINT>(eType)] = std::move(uniquePtr);
	mspComponents[static_cast<UINT>(eType)]->SetOwner(this);
	return *mspComponents[static_cast<UINT>(eType)].get();

}
Component& GameObject::GetComponent(const jh::enums::eComponentType eType)
{
	assert(mspComponents[static_cast<UINT>(eType)] != nullptr);
	return *(mspComponents[static_cast<UINT>(eType)].get());
}

Component* GameObject::GetComponentOrNull(const jh::enums::eComponentType eType)
{
	return mspComponents[static_cast<UINT>(eType)].get();
}

void GameObject::AddScript(const jh::enums::eScriptType eType, std::unique_ptr<Script> uniquePtr)
{
	assert(mspScripts[static_cast<UINT>(eType)] == nullptr);
	mspScripts[static_cast<UINT>(eType)] = std::move(uniquePtr);
	mspScripts[static_cast<UINT>(eType)]->SetOwner(this);
}

Script& GameObject::GetScript(const jh::enums::eScriptType eType)
{
	assert(mspScripts[static_cast<UINT>(eType)] != nullptr);
	return *mspScripts[static_cast<UINT>(eType)].get();
}
Script* GameObject::GetScriptOrNull(const jh::enums::eScriptType eType)
{
	assert(mspScripts[static_cast<UINT>(eType)] != nullptr);
	return mspScripts[static_cast<UINT>(eType)].get();
}

}
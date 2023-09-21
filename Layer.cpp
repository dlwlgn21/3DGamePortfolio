#include "Layer.h"

static constexpr const int MAX_GAME_OBJECT_COUNT = 128;

namespace jh
{
	Layer::Layer()
		: Entity()
	{
		mspGameObjects.reserve(MAX_GAME_OBJECT_COUNT);
	}

	Layer::~Layer()
	{
		mspGameObjects.clear();
	}

	void Layer::Initalize()
	{
		for (auto& spGameObject : mspGameObjects)
		{
			if (spGameObject != nullptr)
			{
				spGameObject->Initialize();
			}
		}
	}

	void Layer::Update()
	{
		for (auto& spGameObject : mspGameObjects)
		{
			if (spGameObject != nullptr)
			{
				spGameObject->Update();
			}
		}
	}

	void Layer::FixedUpdate()
	{
		for (auto& spGameObject : mspGameObjects)
		{
			if (spGameObject != nullptr)
			{
				spGameObject->FixedUpdate();
			}
		}
	}

	void Layer::Render()
	{
		for (auto& spGameObject : mspGameObjects)
		{
			if (spGameObject != nullptr)
			{
				spGameObject->Render();
			}
		}
	}
	 

	void Layer::AddGameObject(std::unique_ptr<GameObject>& spGameObject)
	{
		mspGameObjects.push_back(std::move(spGameObject));
	}
}
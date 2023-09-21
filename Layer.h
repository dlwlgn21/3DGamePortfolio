#pragma once
#include "Entity.h"
#include "GameObject.h"

namespace jh
{
	class GameObject;
	class Layer final : public Entity
	{
	public:
		Layer();
		virtual ~Layer();

		void Initalize();
		void Update();
		void FixedUpdate();
		void Render();

		void AddGameObject(std::unique_ptr<GameObject>& spGameObject);

		std::vector<std::unique_ptr<GameObject>>& GetAllGameObjects() { return mspGameObjects; }
	private:
		std::vector<std::unique_ptr<GameObject>> mspGameObjects;
	};
}


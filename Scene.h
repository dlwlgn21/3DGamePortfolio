#pragma once
#include "Entity.h"
#include "Layer.h"

namespace jh
{
	class GameObject;
	class Scene : public Entity
	{
	public:
		Scene(const jh::enums::eSceneType eType);
		virtual ~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		const jh::enums::eSceneType GetSceneType() const { return meSceneType; }

		Layer& GetLayer(const jh::enums::eLayerType eLayer);
		std::vector<std::unique_ptr<Layer>>& GetAllLayers() { return mLayers; }
		
		void AddGameObject(std::unique_ptr<GameObject>& spGameObject, const jh::enums::eLayerType eType);
		const std::vector<std::unique_ptr<GameObject>>& GetAllGameObjectsFromSpecifiedLayer(const jh::enums::eLayerType eType);
	
	protected:
		std::vector<std::unique_ptr<Layer>>				mLayers;
		jh::enums::eSceneType							meSceneType;
	};
}


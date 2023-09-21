#include "Scene.h"

using namespace jh::enums;

namespace jh
{
	Scene::Scene(const eSceneType eType)
		: Entity()
		, meSceneType(eType)
	{
		mLayers.reserve(static_cast<UINT>(eLayerType::COUNT));

		for (int i = 0; i < static_cast<UINT>(eLayerType::COUNT); ++i)
		{
			mLayers.push_back(std::make_unique<Layer>());
		}
	}

	Scene::~Scene()
	{
		mLayers.clear();
	}

	Layer& Scene::GetLayer(const eLayerType eLayer)
	{
		return *mLayers[static_cast<UINT>(eLayer)].get();
	}

	void Scene::AddGameObject(std::unique_ptr<GameObject>& spGameObject, const eLayerType eType)
	{
		mLayers[static_cast<UINT>(eType)]->AddGameObject(spGameObject);
	}

	const std::vector<std::unique_ptr<GameObject>>& Scene::GetAllGameObjectsFromSpecifiedLayer(const eLayerType eType)
	{
		return mLayers[static_cast<UINT>(eType)]->GetAllGameObjects();
	}


	void Scene::Initialize()
	{
		for (int i = 0; i < static_cast<UINT>(eLayerType::COUNT); ++i)
		{
			if (mLayers[i] != nullptr)
			{
				mLayers[i]->Initalize();
			}
		}
	}
	void Scene::Update()
	{
		for (int i = 0; i < static_cast<UINT>(eLayerType::COUNT); ++i)
		{
			if (mLayers[i] != nullptr)
			{
				mLayers[i]->Update();
			}
		}
	}
	void Scene::FixedUpdate()
	{
		for (int i = 0; i < static_cast<UINT>(eLayerType::COUNT); ++i)
		{
			if (mLayers[i] != nullptr)
			{
				mLayers[i]->FixedUpdate();
			}
		}
	}

	void Scene::Render()
	{
		for (int i = 0; i < static_cast<UINT>(eLayerType::COUNT); ++i)
		{
			if (mLayers[i] != nullptr)
			{
				mLayers[i]->Render();
			}
		}
	}
}
#pragma once
#include "Common.h"

namespace jh
{

class Scene;
class SceneManager final
{
public:
	static SceneManager& GetInstance()
	{
		static SceneManager instance;
		return instance;
	}

	SceneManager(const SceneManager& other) = delete;
	SceneManager& operator=(const SceneManager& other) = delete;

	void Initialize();
	void Update();
	void FixedUpdate();
	void Render();
	void Release();

	Scene& GetCurrentScene() const { assert(mpCurrScene != nullptr); return *mpCurrScene; }

private:
	SceneManager() = default;
	~SceneManager() = default;

private:
	Scene* mpCurrScene = nullptr;
	std::vector<std::unique_ptr<Scene>> mspScenes;
};

}


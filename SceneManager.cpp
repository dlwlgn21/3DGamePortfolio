#include "SceneManager.h"
#include "PlayScene.h"

using namespace jh::enums;

namespace jh
{

void SceneManager::Initialize()
{
	mspScenes.reserve(static_cast<UINT>(eSceneType::COUNT));
	mspScenes.resize(mspScenes.capacity());
	mspScenes[static_cast<UINT>(eSceneType::PLAY_SCENE)] = std::make_unique<PlayScene>();
	mpCurrScene = mspScenes[static_cast<UINT>(eSceneType::PLAY_SCENE)].get();
	mpCurrScene->Initialize();
}
void SceneManager::Update()
{
	assert(mpCurrScene != nullptr);
	mpCurrScene->Update();
}
void SceneManager::FixedUpdate()
{
	assert(mpCurrScene != nullptr);
	mpCurrScene->FixedUpdate();
}

void SceneManager::Render()
{
	assert(mpCurrScene != nullptr);
	mpCurrScene->Render();
}
void SceneManager::Release()
{
	mspScenes.clear(); 
}

}
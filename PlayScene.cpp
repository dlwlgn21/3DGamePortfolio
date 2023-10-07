#include <directxtk/SimpleMath.h>

#include "PlayScene.h"

#include "Time.h"
#include "InputManager.h"
#include "GraphicsPSOManager.h"
#include "ResourcesManager.h"
#include "GraphicsKeyContainer.h"
#include "LightingManager.h"
#include "CameraManager.h"

#include "Mesh.h"
#include "GameObject.h"
#include "Player.h"

#include "Transform.h"
#include "Camera.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Light.h"

using namespace jh::enums;
using namespace jh::graphics;
using namespace DirectX::SimpleMath;

namespace jh
{

PlayScene::PlayScene()
	: Scene(eSceneType::PLAY_SCENE)
{
}
void PlayScene::Initialize()
{
	initCamera();
	initPlayer(); 
	//initBox();
	initWorldCoord();
	initLight();
	Scene::Initialize();
}
void PlayScene::Update()
{
	Scene::Update(); 
}

void PlayScene::initCamera()
{
	std::unique_ptr<GameObject> spCameraGameObject = std::make_unique<GameObject>();
	spCameraGameObject->SetName("Camera");
	spCameraGameObject->AddComponent(eComponentType::CAMERA, std::make_unique<Camera>());
	Camera* pTestCamera = static_cast<Camera*>(spCameraGameObject->GetComponentOrNull(jh::enums::eComponentType::CAMERA));
	spCameraGameObject->AddScript(eScriptType::MOVING, std::make_unique<CameraScript>());

	assert(pTestCamera != nullptr);
	spCameraGameObject->GetTransform().SetPosition(Vector3(0.0f, 0.0f, -2.0f));

	AddGameObject(spCameraGameObject, eLayerType::CAMERA);
	LightingManager::GetInstance().SetCamera(*pTestCamera);
	CameraManager::GetInstance().SetCamera(*pTestCamera);
}

void PlayScene::initPlayer()
{
	{
		std::unique_ptr<GameObject> spSqureGameObject = std::make_unique<GameObject>();
		auto& renderer = static_cast<MeshRenderer&>(spSqureGameObject->AddComponent(eComponentType::RENDERER, std::make_unique<MeshRenderer>()));
		renderer.SetModel(ResourcesManager::Find<jh::graphics::Model>(keys::ZELDA_MODEL));
		AddGameObject(spSqureGameObject, eLayerType::MONSTER);
	}
}

void PlayScene::initWorldCoord()
{
	std::unique_ptr<GameObject> spSqureGameObject = std::make_unique<GameObject>();
	auto& renderer = static_cast<MeshRenderer&>(spSqureGameObject->AddComponent(eComponentType::RENDERER, std::make_unique<MeshRenderer>()));
	renderer.SetModel(ResourcesManager::Find<jh::graphics::Model>(keys::WORLD_COORD_MODEL));
	AddGameObject(spSqureGameObject, eLayerType::PARTICLE);
}

void PlayScene::initBox()
{
	std::unique_ptr<GameObject> spSqureGameObject = std::make_unique<GameObject>();
	auto& renderer = static_cast<MeshRenderer&>(spSqureGameObject->AddComponent(eComponentType::RENDERER, std::make_unique<MeshRenderer>()));
	renderer.SetModel(ResourcesManager::Find<jh::graphics::Model>(keys::BOX_MODEL));
	AddGameObject(spSqureGameObject, eLayerType::MONSTER);
}

void PlayScene::initLight()
{
	std::unique_ptr<GameObject> spSqureGameObject = std::make_unique<GameObject>();
	spSqureGameObject->SetName("Point");
	auto& transform = spSqureGameObject->GetTransform();
	transform.SetPosition(Vector3(0.0f, 0.0f, -10.0f));
	auto& light = static_cast<Light&>(spSqureGameObject->AddComponent(eComponentType::LIGHT, std::make_unique<Light>()));
	light.InitLight(eLightType::POINT);
	auto& renderer = static_cast<MeshRenderer&>(spSqureGameObject->AddComponent(eComponentType::RENDERER, std::make_unique<MeshRenderer>()));
	renderer.SetModel(ResourcesManager::Find<jh::graphics::Model>(keys::SPEHERE_MODEL_KEY));
	AddGameObject(spSqureGameObject, eLayerType::LIGHT); 
}




}

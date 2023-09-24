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
#include "MovingScript.h"
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
	initGround();
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
	spCameraGameObject->GetTransform().SetPosition(Vector3(0.0f, 0.0f, -10.0f));

	AddGameObject(spCameraGameObject, eLayerType::CAMERA);
	LightingManager::GetInstance().SetCamera(*pTestCamera);
	CameraManager::GetInstance().SetCamera(*pTestCamera);
}

void PlayScene::initPlayer()
{
}

void PlayScene::initGround()
{
	std::unique_ptr<GameObject> spSqureGameObject = std::make_unique<GameObject>();
	spSqureGameObject->SetName("Box");
	auto& transform = spSqureGameObject->GetTransform();
	transform.SetPosition(Vector3(0.0f, 0.0f, 5.0f));
	transform.SetScale(Vector3(1.0f, 1.0f, 1.0f));
	
	Material* pMaterial = ResourcesManager::Find<Material>(keys::BASIC_3D_MATERIAL_KEY);
	assert(pMaterial != nullptr);
	auto& renderer = static_cast<MeshRenderer&>(spSqureGameObject->AddComponent(eComponentType::RENDERER, std::make_unique<MeshRenderer>()));
	renderer.SetMesh(GraphicsPSOManager::GetInstance().GetSpeherMeshPtr());
	renderer.SetMaterial(pMaterial);
	AddGameObject(spSqureGameObject, eLayerType::MONSTER);
}

void PlayScene::initLight()
{
	std::unique_ptr<GameObject> spSqureGameObject = std::make_unique<GameObject>();
	spSqureGameObject->SetName("DirectionalLight");
	auto& transform = spSqureGameObject->GetTransform();
	transform.SetPosition(Vector3(0.0f, 0.0f, -2.0f));
	auto& light = static_cast<Light&>(spSqureGameObject->AddComponent(eComponentType::LIGHT, std::make_unique<Light>()));
	light.InitLight(eLightType::DIRECTIONAL);
	AddGameObject(spSqureGameObject, eLayerType::LIGHT);
}


}

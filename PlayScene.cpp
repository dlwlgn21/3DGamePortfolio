#include <directxtk/SimpleMath.h>

#include "PlayScene.h"

#include "Time.h"
#include "InputManager.h"
#include "GraphicsPSOManager.h"
#include "ResourcesManager.h"
#include "GraphicsKeyContainer.h"
#include "LightingManager.h"
#include "CameraManager.h"
#include "AnimationDataManager.h"
#include "PlayerManager.h"

#include "Mesh.h"
#include "GameObject.h"
#include "Player.h"

#include "Transform.h"
#include "Camera.h"
#include "CameraScript.h"
#include "Collider3D.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "BoneAnimator.h"
#include "PlayerScript.h"
#include "MonsterScript.h"
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
	initMutant();
	initWorldCoord();
	initEnv();
	//initGround();
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
	auto* pAnimData = AnimationDataManager::GetInstance().GetAnimDataOrNull(AnimationDataManager::BASIC_CHARACTER_MORTION_ANIM_DATA_KEY);
	assert(pAnimData != nullptr);
	std::unique_ptr<GameObject> spSqureGameObject = std::make_unique<GameObject>();
	auto& renderer = static_cast<MeshRenderer&>(spSqureGameObject->AddComponent(eComponentType::RENDERER, std::make_unique<SkinnedMeshRenderer>()));
	renderer.SetModel(ResourcesManager::Find<jh::graphics::Model>(keys::BARNY_SKINNED_MODEL));
		
	auto& animator = static_cast<BoneAnimator&>(spSqureGameObject->AddComponent(eComponentType::ANIMATOR, std::make_unique<BoneAnimator>()));
	animator.InitAnimationData(pAnimData, eAnimClipKeyContainerType::PLAYER);
	ResourcesManager::Find<Model>(keys::BARNY_SKINNED_MODEL)->SetBoneAnimator(animator);
	auto& collider = static_cast<Collider3D&>(spSqureGameObject->AddComponent(eComponentType::COLLIDER_3D, std::make_unique<Collider3D>()));
	collider.InitHitBox(Vector3(0.0f, 0.4f, 0.0f), Vector3(0.2f, 0.7f, 0.2f));
	collider.InitBoundingSphere(Vector3(-0.2f, 0.5f, -0.5f), 0.5f, eBoundingSphereType::ATTACK_SPHERE);
	collider.InitAttackBox(Vector3(0.0f, 0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));
	spSqureGameObject->AddScript(eScriptType::HEAD, std::make_unique<PlayerScript>());
	auto& script = static_cast<PlayerScript&>(spSqureGameObject->GetScript(eScriptType::HEAD));
	PlayerManager::GetInstance().SetPlayerScript(script);
	script.SetBoneAnimator(&animator);
	spSqureGameObject->GetTransform().SetPosition(Vector3(-1.0f, 0.0f, 0.0f));
	AddGameObject(spSqureGameObject, eLayerType::PLAYER);
}

void PlayScene::initMutant()
{
	auto* pAnimData = AnimationDataManager::GetInstance().GetAnimDataOrNull(AnimationDataManager::BASIC_MONSTER_MORTION_ANIM_DATA_KEY);
	assert(pAnimData != nullptr);
	std::unique_ptr<GameObject> spSqureGameObject = std::make_unique<GameObject>();
	auto& renderer = static_cast<MeshRenderer&>(spSqureGameObject->AddComponent(eComponentType::RENDERER, std::make_unique<SkinnedMeshRenderer>()));
	renderer.SetModel(ResourcesManager::Find<jh::graphics::Model>(keys::MUTANT_SKINNED_MODEL));

	auto& animator = static_cast<BoneAnimator&>(spSqureGameObject->AddComponent(eComponentType::ANIMATOR, std::make_unique<BoneAnimator>()));
	animator.InitAnimationData(pAnimData, eAnimClipKeyContainerType::MONSTER);
	ResourcesManager::Find<Model>(keys::MUTANT_SKINNED_MODEL)->SetBoneAnimator(animator);
	auto& collider = static_cast<Collider3D&>(spSqureGameObject->AddComponent(eComponentType::COLLIDER_3D, std::make_unique<Collider3D>()));
	collider.InitHitBox(Vector3(0.0f, 0.4f, -0.1f), Vector3(0.2f, 0.4f, 0.2f));
	collider.InitBoundingSphere(Vector3(0.0f, 0.3f, -0.3f), 0.15f, eBoundingSphereType::ATTACK_SPHERE);
	collider.InitBoundingSphere(Vector3(0.0f, 0.0f, 0.0f), 5.5f, eBoundingSphereType::AGRO_SPHERE);
	spSqureGameObject->AddScript(eScriptType::HEAD, std::make_unique<MonsterScript>());
	auto& script = static_cast<MonsterScript&>(spSqureGameObject->GetScript(eScriptType::HEAD));
	script.SetBoneAnimator(&animator);
	spSqureGameObject->GetTransform().SetPosition(Vector3(-1.0f, 0.0f, -10.0f));
	spSqureGameObject->GetTransform().SetScale(Vector3(1.5f, 1.5f, 1.5f));
	AddGameObject(spSqureGameObject, eLayerType::MONSTER);
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

void PlayScene::initEnv()
{
	std::unique_ptr<GameObject> spSqureGameObject = std::make_unique<GameObject>();
	spSqureGameObject->SetName("Catsle");
	spSqureGameObject->GetTransform().SetPosition(Vector3(0.0f, 1.2f, 0.0f));
	spSqureGameObject->GetTransform().SetScale(Vector3(30.0f, 30.0f, 30.0f));
	auto& renderer = static_cast<MeshRenderer&>(spSqureGameObject->AddComponent(eComponentType::RENDERER, std::make_unique<MeshRenderer>()));
	renderer.SetModel(ResourcesManager::Find<jh::graphics::Model>(keys::CATSLE_MODEL_KEY));
	AddGameObject(spSqureGameObject, eLayerType::ENV);
}

void PlayScene::initLight()
{
	std::unique_ptr<GameObject> spSqureGameObject = std::make_unique<GameObject>();
	spSqureGameObject->SetName("Point");
	auto& transform = spSqureGameObject->GetTransform();
	//transform.SetPosition(Vector3(0.0f, 0.0f, -10.0f));
	auto& light = static_cast<Light&>(spSqureGameObject->AddComponent(eComponentType::LIGHT, std::make_unique<Light>()));
	light.InitLight(eLightType::POINT);
	AddGameObject(spSqureGameObject, eLayerType::LIGHT); 
}

void PlayScene::initGround()
{
	std::unique_ptr<GameObject> spSqureGameObject = std::make_unique<GameObject>();
	spSqureGameObject->SetName("Ground");
	spSqureGameObject->GetTransform().SetPosition(Vector3(0.0f, 1.0f, 0.0f));
	auto& renderer = static_cast<MeshRenderer&>(spSqureGameObject->AddComponent(eComponentType::RENDERER, std::make_unique<MeshRenderer>()));
	renderer.SetModel(ResourcesManager::Find<jh::graphics::Model>(keys::GROUND_MODEL));
	AddGameObject(spSqureGameObject, eLayerType::ENV);
}



}

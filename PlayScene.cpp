#include <directxtk/SimpleMath.h>

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

#include "PlayScene.h"

#include "Time.h"
#include "InputManager.h"
#include "GraphicsPSOManager.h"
#include "ResourcesManager.h"
#include "GraphicsKeyContainer.h"
#include "Box2DPhysicsManager.h"

#include "Mesh.h"
#include "GameObject.h"
#include "Player.h"


#include "Transform.h"
#include "Camera.h"
#include "CameraScript.h"
#include "MovingScript.h"
#include "MeshRenderer.h"
#include "RigidBody2D.h"
#include "BoxCollider2D.h"

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
	Scene::Initialize();
}
void PlayScene::Update()
{
	Scene::Update(); 
}

void PlayScene::initCamera()
{
	std::unique_ptr<GameObject> spCameraGameObject = std::make_unique<GameObject>();
	spCameraGameObject->AddComponent(eComponentType::CAMERA, std::make_unique<Camera>());
	Camera* pTestCamera = static_cast<Camera*>(spCameraGameObject->GetComponentOrNull(jh::enums::eComponentType::CAMERA));
	spCameraGameObject->AddScript(eScriptType::MOVING, std::make_unique<CameraScript>());

	assert(pTestCamera != nullptr);
	spCameraGameObject->GetTransform().SetPosition(Vector3(0.0f, 0.0f, -10.0f));

	AddGameObject(spCameraGameObject, eLayerType::CAMERA); 
}

void PlayScene::initPlayer()
{
	std::unique_ptr<GameObject> spSqureGameObject = std::make_unique<Player>();
	AddGameObject(spSqureGameObject, eLayerType::PLAYER);
}

void PlayScene::initGround()
{
	std::unique_ptr<GameObject> spSqureGameObject = std::make_unique<GameObject>();
	spSqureGameObject->SetName("Ground");
	auto& transform = spSqureGameObject->GetTransform();
	transform.SetPosition(Vector3(0.0f, -10.0f, 0.0f));
	transform.SetScale(Vector3(30.0f, 30.0f, 1.0f));
	const Vector2 pos2D = transform.GetOnlyXYPosition();
	const Vector3 scale = transform.GetScale();
	 
	MeshRenderer& mr = static_cast<MeshRenderer&>(spSqureGameObject->AddComponent(eComponentType::RENDERER, std::make_unique<MeshRenderer>()));
	mr.SetMesh(GraphicsPSOManager::GetInstance().GetSqureMeshPtr());
	mr.SetMaterial(ResourcesManager::Find<Material>(keys::BASIC_2D_MATERIAL_KEY));
	RigidBody2D& rb2D = static_cast<RigidBody2D&>(spSqureGameObject->AddComponent(eComponentType::RIGID_BODY_2D, std::make_unique<RigidBody2D>()));
	rb2D.SetRBType(eRigidBodyType::STATIC);
	BoxCollider2D& box2D = static_cast<BoxCollider2D&>(spSqureGameObject->AddComponent(eComponentType::BOX_COLLIDER_2D, std::make_unique<BoxCollider2D>()));
	box2D.InitFixture(*rb2D.GetB2BodyOrNull(), eColliderFilter::GROUND, eColliderFilter::ALL);
	AddGameObject(spSqureGameObject, eLayerType::MONSTER);
}

}

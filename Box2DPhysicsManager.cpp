#include <directxtk/SimpleMath.h>

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_draw.h>

#include "Box2DPhysicsManager.h"
#include "Box2DDebugDrawManager.h"
#include "GameObject.h"
#include "Layer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Transform.h"
#include "RigidBody2D.h"

using namespace jh::enums;

namespace jh
{

void Box2DManager::Initialize()
{
	const b2Vec2 gravity(0.0f, -10.0f);
	mpB2World = new b2World(gravity);
	Box2DDebugDrawManager& drawManager = Box2DDebugDrawManager::GetInstance();
	drawManager.Initialize();
	mpB2World->SetDebugDraw(&drawManager);
}

void Box2DManager::Update()
{
	
}

void Box2DManager::FixedUpdate()
{
	mpB2World->Step(mFixedDeltaTime, mVelocityInterations, mPositionInterations);
	updateGameObjectTransformAfterBox2DSimulating();
}

void Box2DManager::Render()
{
	mpB2World->DebugDraw();
}

void Box2DManager::Release()
{
	if (mpB2World != nullptr)
	{
		delete mpB2World;
		mpB2World = nullptr;
	}
}

b2World& Box2DManager::GetB2World()
{
	return *mpB2World;
}

void Box2DManager::updateGameObjectTransformAfterBox2DSimulating()
{
	b2Body* pIter = mpB2World->GetBodyList();
	while (pIter != nullptr)
	{
		GameObject* pGameObject = reinterpret_cast<GameObject*>(pIter->GetUserData().pointer);
		if (pGameObject != nullptr) {
			std::string name = pGameObject->GetName();
			pGameObject->GetTransform().Set2DPosition(pIter->GetPosition().x, pIter->GetPosition().y);
			pGameObject->GetTransform().Set2DRotation(DirectX::XMConvertToDegrees(pIter->GetAngle()));
			//pGameObject->m_linearVelocity = pIter->GetLinearVelocity();
		}
		pIter = pIter->GetNext();
	}

	//auto& layers = SceneManager::GetInstance().GetCurrentScene().GetAllLayers();
	//for (auto& layer : layers)
	//{
	//	auto& gameObjects = layer->GetAllGameObjects();
	//	if (gameObjects.size() != 0)
	//	{
	//		for (auto& gameObject : gameObjects)
	//		{
	//			if (gameObject->GetComponentOrNull(eComponentType::RIGID_BODY_2D) != nullptr)
	//			{
	//				auto& transfrom = gameObject->GetTransform();
	//				auto& rb2D =  static_cast<RigidBody2D&>(gameObject->GetComponent(eComponentType::RIGID_BODY_2D));
	//				
	//				const b2Body& pB2Body = rb2D.GetBody();
	//				b2Vec2 bodyPosition = pB2Body.GetPosition();
	//				transfrom.Set2DPosition(bodyPosition.x, bodyPosition.y);
	//				transfrom.Set2DRotation(DirectX::XMConvertToDegrees(pB2Body.GetAngle()));
	//			}
	//		}
	//	}
	//}
}

}
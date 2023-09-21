#include "Player.h"

#include "GraphicsPSOManager.h"
#include "GraphicsKeyContainer.h"
#include "ResourcesManager.h"

#include "Texture.h"
#include "Animation.h"

#include "Animator.h"
#include "BoxCollider2D.h"
#include "MovingScript.h"
#include "MeshRenderer.h"
#include "RigidBody2D.h"
#include "PlayerScript.h"

using namespace jh::enums;
using namespace DirectX::SimpleMath;
using namespace jh::graphics;
using namespace jh::graphics::keys;

namespace jh
{
	const std::string Player::IDLE_ANIM_KEY = "i";
	const std::string Player::RUN_ANIM_KEY = "r";
	const std::string Player::ATTACK_1_ANIM_KEY = "a";
	const std::string Player::ATTACK_2_ANIM_KEY = "a2";
	const std::string Player::ATTACK_3_ANIM_KEY = "a3";
	const std::string Player::ATTACK_PUSH_ANIM_KEY = "a4";
	const std::string Player::PLAYER_ROLLING_KEY = "d";
	const std::string Player::PLAYER_DASH_KEY = "l";
	const std::string Player::PLAYER_HITTED_KEY = "h";

	Player::Player()
		: MoveableGameObject()
	{
		SetName("Player");
		Transform& transform = GetTransform();
		transform.SetRotation(Vector3(0.0f, 0.0f, 0.0f));
		transform.SetPosition(Vector3(0.0f, 10.0f, -1.0f));
		initRenderer();
		initPhysics();
		initAnimator();
		initScript();
	}

	void Player::initRenderer()
	{
		MeshRenderer& mr = static_cast<MeshRenderer&>(AddComponent(eComponentType::RENDERER, std::make_unique<MeshRenderer>()));
		mr.SetMesh(GraphicsPSOManager::GetInstance().GetSqureMeshPtr());
		mr.SetMaterial(ResourcesManager::Find<Material>(keys::BASIC_2D_MATERIAL_KEY));
	}
	void Player::initScript()
	{
		AddScript(eScriptType::MOVING, std::make_unique<MovingScript>());
		AddScript(eScriptType::HEAD, std::make_unique<PlayerScript>());
	}
	void Player::initAnimator()
	{
		Texture* pTex = ResourcesManager::Find<Texture>(keys::PLAYER_SPRITE_SHEET_TEXTURE_KEY);
		assert(pTex != nullptr);
		const float WIDTH = 90.0f;
		const float HEIGHT = 37.0f;
		const float MAGNINICATION = 80.0f;
		const Vector2 ANIM_SIZE(WIDTH, HEIGHT);
		const Vector2 OFFSET(0.02f, 0.0f);
		const float PLAYER_ATTACK_DURAITON = 0.1f;

		auto& animator = static_cast<Animator&>(AddComponent(eComponentType::ANIMATOR, std::make_unique<Animator>()));
		animator.Create(
			IDLE_ANIM_KEY,
			*pTex,
			Vector2(0.0f, HEIGHT * 1),
			ANIM_SIZE,
			OFFSET,
			9,
			0.1f,
			MAGNINICATION
		);

		animator.Create(
			RUN_ANIM_KEY,
			*pTex,
			Vector2(0.0f, HEIGHT * 4),
			ANIM_SIZE,
			OFFSET,
			8,
			0.1f,
			MAGNINICATION
		);

		animator.Create(
			ATTACK_1_ANIM_KEY,
			*pTex,
			Vector2(0.0f, HEIGHT * 7),
			ANIM_SIZE,
			OFFSET,
			7,
			PLAYER_ATTACK_DURAITON,
			MAGNINICATION
		);

		animator.Create(
			ATTACK_2_ANIM_KEY,
			*pTex,
			Vector2(0.0f, HEIGHT * 8),
			ANIM_SIZE,
			OFFSET,
			5,
			PLAYER_ATTACK_DURAITON,
			MAGNINICATION
		);
		animator.Create(
			ATTACK_3_ANIM_KEY,
			*pTex,
			Vector2(0.0f, HEIGHT * 9),
			ANIM_SIZE,
			OFFSET,
			5,
			PLAYER_ATTACK_DURAITON + 0.03f,
			MAGNINICATION
		);


		animator.Create(
			ATTACK_PUSH_ANIM_KEY,
			*pTex,
			Vector2(0.0f, HEIGHT * 10),
			ANIM_SIZE,
			OFFSET,
			6,
			0.1f,
			MAGNINICATION
		);

		animator.Create(
			PLAYER_ROLLING_KEY,
			*pTex,
			Vector2(0.0f, HEIGHT * 5),
			ANIM_SIZE,
			OFFSET,
			8,
			0.075f,
			MAGNINICATION
		);

		animator.Create(
			PLAYER_DASH_KEY,
			*pTex,
			Vector2(0.0f, HEIGHT * 12),
			ANIM_SIZE,
			OFFSET,
			6,
			0.075f,
			MAGNINICATION
		);

		animator.Create(
			PLAYER_HITTED_KEY,
			*pTex,
			Vector2(0.0f, HEIGHT * 25),
			ANIM_SIZE,
			OFFSET,
			2,
			0.1f,
			MAGNINICATION
		);

		animator.PlayAnimation(IDLE_ANIM_KEY, true);
	}
	void Player::initPhysics()
	{
		RigidBody2D& rb2D = static_cast<RigidBody2D&>(AddComponent(eComponentType::RIGID_BODY_2D, std::make_unique<RigidBody2D>()));
		rb2D.SetRBType(eRigidBodyType::DYNAMIC);
		BoxCollider2D& box2D = static_cast<BoxCollider2D&>(AddComponent(eComponentType::BOX_COLLIDER_2D, std::make_unique<BoxCollider2D>()));
		box2D.InitFixture(*rb2D.GetB2BodyOrNull(), eColliderFilter::PLAYER, eColliderFilter::GROUND | eColliderFilter::MONSTER);
	}
}
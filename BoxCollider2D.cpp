#include <box2d/b2_fixture.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include "BoxCollider2D.h"
#include "GameObject.h"
#include "Transform.h"

using namespace jh::enums;
using namespace DirectX::SimpleMath;

namespace jh
{


BoxCollider2D::BoxCollider2D()
	: Component(eComponentType::BOX_COLLIDER_2D)
{
}

void BoxCollider2D::Initialize()
{
}

void BoxCollider2D::Update()
{
}

void BoxCollider2D::FixedUpdate()
{ 
}

void BoxCollider2D::InitFixture(b2Body& body, const unsigned short categoryBit, const unsigned short maskBit)
{
	const auto& scale = GetOwner().GetTransform().GetScale();
	SetSize(Vector2(scale.x, scale.y));
	b2PolygonShape boxShape;
	b2FixtureDef fixtureDef;
	boxShape.SetAsBox(GetSize().x, GetSize().y);
	fixtureDef.shape = &boxShape;
	fixtureDef.density = mDensity;
	fixtureDef.friction = mFriction;
	fixtureDef.restitution = mRestitution;
	fixtureDef.restitutionThreshold = mRestitutionThreshold;
	fixtureDef.filter.categoryBits = categoryBit;
	fixtureDef.filter.maskBits = maskBit;
	meCategoryFilter = static_cast<jh::enums::eColliderFilter>(categoryBit);
	meMaskFilter = static_cast<jh::enums::eColliderFilter>(maskBit);
	body.CreateFixture(&fixtureDef);
}

}
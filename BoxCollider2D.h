#pragma once
#include "Component.h"

class b2Body;

namespace jh
{

class BoxCollider2D : public Component
{

public:
	BoxCollider2D();
	virtual ~BoxCollider2D() = default;

	virtual void Initialize();
	virtual void Update();
	virtual void FixedUpdate();

	void InitFixture(b2Body& body, const unsigned short categoryBit, const unsigned short  maskBit);

	void SetOffset(DirectX::SimpleMath::Vector2 offset)		{ mOffset = offset; }
	void SetSize(const DirectX::SimpleMath::Vector2 size) { mSize.x = size.x * 0.5f; mSize.y = size.y * 0.5f; }

	DirectX::SimpleMath::Vector2 GetOffset() const			{ return mOffset; }
	DirectX::SimpleMath::Vector2 GetSize() const			{ return mSize; }
	float GetDensity() const								{ return mDensity; }
	float GetFriction() const								{ return mFriction; }
	float GetRestitution() const							{ return mRestitution; }
	float GetRestitutionThreshold() const					{ return mRestitutionThreshold; }

	jh::enums::eColliderFilter GetCagegoryFilter() const	{ return meCategoryFilter; }
	jh::enums::eColliderFilter GetMaskFilter() const		{ return meMaskFilter; }

private:
	DirectX::SimpleMath::Vector2 mOffset = { 0.0f, 0.0f };
	DirectX::SimpleMath::Vector2 mSize = { 0.5f, 0.5f };
	float mDensity = 1.0f;
	float mFriction = 0.5f;
	float mRestitution = 0.3f;
	float mRestitutionThreshold = 0.5f;
	jh::enums::eColliderFilter meCategoryFilter = jh::enums::eColliderFilter::NONE;
	jh::enums::eColliderFilter meMaskFilter = jh::enums::eColliderFilter::NONE;

};

}


#pragma once
#include "Component.h"
#include "Model.h"
#include <directxtk/PrimitiveBatch.h>
#include <directxtk/VertexTypes.h>
#include <DirectXCollision.h>


#include <directxtk/Effects.h>

namespace jh
{
using namespace DirectX;

enum class eBoundingBoxType
{
	HIT_BOX,
	COUNT
};

enum class eBoundingSphereType
{
	ATTACK_SPHERE,
	AGRO_SPHERE,
	COUNT
};

class Collider3D final : public Component
{
public:
	Collider3D();
	virtual ~Collider3D() = default;

	void FixedUpdate() override;

	void InitHitBox(const DirectX::SimpleMath::Vector3& offset, const DirectX::SimpleMath::Vector3& extent);
	void InitAttackBox(const DirectX::SimpleMath::Vector3& offset, const DirectX::SimpleMath::Vector3& extent);
	void InitBoundingSphere(const DirectX::SimpleMath::Vector3& offset, const float radius, const eBoundingSphereType eType);
	void Render();

	DirectX::BoundingBox& GetHitBox();
	DirectX::BoundingSphere& GetSphere(const eBoundingSphereType eType);

private:
	DirectX::BoundingBox													mHitBox;
	DirectX::SimpleMath::Vector3											mHitBoxOffset;
	DirectX::BoundingSphere													mSpheres[static_cast<UINT>(eBoundingSphereType::COUNT)];
	DirectX::BoundingOrientedBox											mOrientedAttackBox;
	DirectX::SimpleMath::Vector3											mAttackBoxOffset;
	DirectX::SimpleMath::Vector3											mSphereOffsets[static_cast<UINT>(eBoundingSphereType::COUNT)];
};

}


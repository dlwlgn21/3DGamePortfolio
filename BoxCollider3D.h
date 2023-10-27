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
	ATTACK_BOX,
	AGRO_BOX,
	COUNT
};

class BoxCollider3D final : public Component
{
public:
	BoxCollider3D();
	virtual ~BoxCollider3D() = default;

	void FixedUpdate() override;

	void InitBondingBox(const DirectX::SimpleMath::Vector3& centerPos, const DirectX::SimpleMath::Vector3& extent, const eBoundingBoxType eBoxTpye);
	void InitBoxScale(const DirectX::SimpleMath::Vector3& scale) { mBoxScale = scale; }
	void Render();

	DirectX::BoundingBox& GetBoundingBox(const eBoundingBoxType eBoxType);


private:
	DirectX::SimpleMath::Vector3											mBoxScale;
	DirectX::BoundingBox													mBoundingBoxes[static_cast<UINT>(eBoundingBoxType::COUNT)];
};

}


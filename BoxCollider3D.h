#pragma once
#include "Component.h"
#include "Model.h"
#include <directxtk/PrimitiveBatch.h>
#include <directxtk/VertexTypes.h>
#include <DirectXCollision.h>

namespace jh
{
using namespace DirectX;

enum class eBoundingBoxType
{
	HIT_BOX,
	ATTACK_BOX,
	COUNT
};

class BoxCollider3D final : public Component
{
public:
	BoxCollider3D();
	virtual ~BoxCollider3D() = default;

	void InitBondingBox(const DirectX::SimpleMath::Vector3& centerPos, const DirectX::SimpleMath::Vector3& extent, const eBoundingBoxType eBoxTpye);
	void InitBoxScale(const DirectX::SimpleMath::Vector3& scale) { mBoxScale = scale; }
	void SetModel(jh::graphics::Model* pModel) { assert(pModel != nullptr); mpDebugDrawModel = pModel; }
	void Render();

private:
	jh::graphics::Model*													mpDebugDrawModel;
	DirectX::SimpleMath::Vector3											mBoxScale;
	DirectX::BoundingBox													mBoundingBoxes[static_cast<UINT>(eBoundingBoxType::COUNT)];
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>	mspPrimitiveBatch;
};

}


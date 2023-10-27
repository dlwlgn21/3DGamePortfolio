#include "BoxCollider3D.h"

#include "GameObject.h"
#include "Transform.h"
#include "GraphicDeviceDX11.h"
#include "GraphicsPSOManager.h"

#include "DebugDraw.h"

#include "ColliderDebugRenderer.h"

using namespace jh::enums;
using namespace jh::graphics;
using namespace DirectX;
using namespace DirectX::SimpleMath;
namespace jh
{

BoxCollider3D::BoxCollider3D()
	: Component(eComponentType::BOX_COLLIDER_3D)
	, mBoundingBoxes()
	, mBoxScale(Vector3(1.0f, 1.0f, 1.0f))
{
	for (UINT i = 0; i < static_cast<UINT>(eBoundingBoxType::COUNT); ++i)
	{
		mBoundingBoxes[i].Extents = Vector3(0.0f, 0.0f, 0.0f);
	}

}

void BoxCollider3D::InitBondingBox(const DirectX::SimpleMath::Vector3& centerPos, const DirectX::SimpleMath::Vector3& extent, const eBoundingBoxType eBoxTpye)
{
	mBoundingBoxes[static_cast<UINT>(eBoxTpye)].Center = centerPos;
	mBoundingBoxes[static_cast<UINT>(eBoxTpye)].Extents = Vector3(extent.x * 0.5f, extent.y * 0.5f, extent.z * 0.5f);
}


DirectX::BoundingBox& BoxCollider3D::GetBoundingBox(const eBoundingBoxType eBoxType)
{
	assert(mBoundingBoxes[static_cast<UINT>(eBoxType)].Extents != Vector3(0.0f, 0.0f, 0.0f));
	return mBoundingBoxes[static_cast<UINT>(eBoxType)];
}

void BoxCollider3D::FixedUpdate()
{
	for (UINT i = 0; i < static_cast<UINT>(eBoundingBoxType::COUNT); ++i)
	{
		mBoundingBoxes[i].Center = GetOwner().GetTransform().GetPosition();
	}
}


void BoxCollider3D::Render()
{
	using VertexType = DirectX::VertexPositionColor;

	auto& colliderRenderer = ColliderDebugRenderer::GetInstance();
	for (UINT i = 0; i < static_cast<UINT>(eBoundingBoxType::COUNT); ++i)
	{
		switch (i)
		{
		case static_cast<UINT>(eBoundingBoxType::HIT_BOX):
		{
			colliderRenderer.BoundingBoxRender(mBoundingBoxes[static_cast<UINT>(i)], DirectX::Colors::Purple);
			break;
		}
		case static_cast<UINT>(eBoundingBoxType::ATTACK_BOX):
		{
			colliderRenderer.BoundingBoxRender(mBoundingBoxes[static_cast<UINT>(i)], DirectX::Colors::GreenYellow);
			break;
		}
		case static_cast<UINT>(eBoundingBoxType::AGRO_BOX):
		{
			colliderRenderer.BoundingBoxRender(mBoundingBoxes[static_cast<UINT>(i)], DirectX::Colors::AntiqueWhite);
			break;
		}
		default:
			break;
		}
	}
}


}


#include "BoxCollider3D.h"
#include "GameObject.h"
#include "Transform.h"
#include "GraphicDeviceDX11.h"
#include "DebugDraw.h"

using namespace jh::enums;
using namespace jh::graphics;
using namespace DirectX;
using namespace DirectX::SimpleMath;
namespace jh
{

BoxCollider3D::BoxCollider3D()
	: Component(eComponentType::BOX_COLLIDER_3D)
	, mpDebugDrawModel(nullptr)
	, mBoundingBoxes()
	, mBoxScale(Vector3(1.0f, 1.0f, 1.0f))
	, mspPrimitiveBatch()
{
	mspPrimitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(&GraphicDeviceDX11::GetInstance().GetDeivceContext());
	mBoundingBoxes[0].Extents = Vector3(0.0f, 0.0f, 0.0f);
	mBoundingBoxes[1].Extents = Vector3(0.0f, 0.0f, 0.0f);


}

void BoxCollider3D::InitBondingBox(const DirectX::SimpleMath::Vector3& centerPos, const DirectX::SimpleMath::Vector3& extent, const eBoundingBoxType eBoxTpye)
{
	mBoundingBoxes[static_cast<UINT>(eBoxTpye)].Center = centerPos;
	mBoundingBoxes[static_cast<UINT>(eBoxTpye)].Extents = Vector3(extent.x * 0.5f, extent.y * 0.5f, extent.z * 0.5f);
}
void BoxCollider3D::Render()
{
	assert(mpDebugDrawModel != nullptr);
	//const auto& originalScale = GetOwner().GetTransform().GetScale();
	//GetOwner().GetTransform().SetScale(mBoxScale);
	//GetOwner().GetTransform().UpdateConstantBuffer();
	mpDebugDrawModel->SetPipeline();
	mspPrimitiveBatch->Begin();
	for (UINT i = 0; i < static_cast<UINT>(eBoundingBoxType::COUNT); ++i)
	{
		if (mBoundingBoxes[i].Extents == Vector3(0.0f, 0.0f, 0.0f))
		{
			continue;
		}
		if (i == static_cast<UINT>(eBoundingBoxType::HIT_BOX))
		{
			debug_draw::Draw(mspPrimitiveBatch.get(), mBoundingBoxes[i], DirectX::Colors::Blue);
		}
		else if (i == static_cast<UINT>(eBoundingBoxType::ATTACK_BOX))
		{
			debug_draw::Draw(mspPrimitiveBatch.get(), mBoundingBoxes[i], DirectX::Colors::OrangeRed);
		}
	}
	mspPrimitiveBatch->End();
	//GetOwner().GetTransform().SetScale(originalScale);
}

}

#include "BoxCollider3D.h"
#include "GameObject.h"
#include "Transform.h"
#include "GraphicDeviceDX11.h"

using namespace jh::enums;
using namespace jh::graphics;
using namespace DirectX;
using namespace DirectX::SimpleMath;
namespace jh
{


BoxCollider3D::BoxCollider3D()
	: Component(eComponentType::BOX_COLLIDER_3D)
	, mpDebugDrawModel(nullptr)
	, mBoxScale(Vector3(1.0f, 1.0f, 1.0f))
{
	//mspPrimitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(&GraphicDeviceDX11::GetInstance().GetDeivceContext());
}

void BoxCollider3D::Render()
{
	const auto& originalScale = GetOwner().GetTransform().GetScale();
	GetOwner().GetTransform().SetScale(mBoxScale);
	GetOwner().GetTransform().UpdateConstantBuffer();
	mpDebugDrawModel->Render();
	GetOwner().GetTransform().SetScale(originalScale);
}

}

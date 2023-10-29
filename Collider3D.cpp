#include "Collider3D.h"

#include "GraphicDeviceDX11.h"
#include "GraphicsPSOManager.h"
#include "ColliderDebugRenderer.h"
#include "Time.h"
#include "D3DApp.h"

#include "GameObject.h"
#include "Transform.h"

#include "DebugDraw.h"

#include "DebugHelper.h"

using namespace jh::enums;
using namespace jh::graphics;
using namespace DirectX;
using namespace DirectX::SimpleMath;
namespace jh
{
Collider3D::Collider3D()
	: Component(eComponentType::COLLIDER_3D)
	, mHitBox()
	, mHitBoxOffset()
	, mSpheres()
	, mSphereOffsets()

{

}

void Collider3D::InitHitBox(const DirectX::SimpleMath::Vector3& offset, const DirectX::SimpleMath::Vector3& extent)
{
	mHitBoxOffset = offset;
	mHitBox.Extents = Vector3(extent.x * 0.5f, extent.y * 0.5f, extent.z * 0.5f);
}

void Collider3D::InitAttackBox(const DirectX::SimpleMath::Vector3& offset, const DirectX::SimpleMath::Vector3& extent)
{
	mAttackBoxOffset = offset;
	mOrientedAttackBox.Extents = Vector3(extent.x * 0.5f, extent.y * 0.5f, extent.z * 0.5f);
}
void Collider3D::InitBoundingSphere(const DirectX::SimpleMath::Vector3& offset, const float radius, const eBoundingSphereType eType)
{
	mSphereOffsets[static_cast<UINT>(eType)] = offset;
	mSpheres[static_cast<UINT>(eType)].Radius = radius;
}

DirectX::BoundingBox& Collider3D::GetHitBox()
{
	return mHitBox;
}

DirectX::BoundingSphere& Collider3D::GetSphere(const eBoundingSphereType eType)
{
	return mSpheres[static_cast<UINT>(eType)];
}

void Collider3D::FixedUpdate()
{
	auto& tr = GetOwner().GetTransform();
	Matrix rMat = Matrix::CreateRotationY(DirectX::XMConvertToRadians(tr.GetYawDeg()));

	for (UINT i = 0; i < static_cast<UINT>(eBoundingBoxType::COUNT); ++i)
	{
		mHitBox.Center = tr.GetPosition() + mHitBoxOffset;
	}
	//{
	//	Vector3 direction = (mAttackBoxOffset + tr.GetPosition()) - tr.GetPosition();
	//	direction.Normalize();
	//	Vector3 rotatedDirection = Vector3::Transform(direction, rMat);
	//	Vector3 pos = tr.GetPosition() + rotatedDirection;
	//	mOrientedAttackBox.Center = pos;
	//}

	for (UINT i = 0; i < static_cast<UINT>(eBoundingSphereType::COUNT); ++i)
	{
		Vector3 direction = (mSphereOffsets[i] + tr.GetPosition()) - tr.GetPosition();
		direction.Normalize();
		Vector3 rotatedDirection = Vector3::Transform(direction, rMat);
		Vector3 pos = tr.GetPosition() + rotatedDirection;
		mSpheres[i].Center = pos;
	}
}


void Collider3D::Render()
{
	if (!D3DApp::GetInstance().IsDrawCollision())
	{
		return;
	}

	auto& tr = GetOwner().GetTransform();
	auto& colliderRenderer = ColliderDebugRenderer::GetInstance();
	//Vector3 centerToOffset = mAttackBoxOffset - Vector3(0.0f, 0.0f, 0.0f);
	//centerToOffset.Normalize();
	//Matrix rMat = Matrix::CreateRotationY(DirectX::XMConvertToRadians(tr.GetYawDeg()));
	//Vector3 rotatedRay = Vector3::Transform(centerToOffset, rMat);
	//colliderRenderer.RayRender(Vector3(GetOwner().GetTransform().GetPosition()), rotatedRay, Colors::Blue);
	colliderRenderer.RayRender(tr.GetPosition(), tr.GetForwardRef(), Colors::Blue);
	colliderRenderer.BoundingBoxRender(mHitBox, DirectX::Colors::Purple);
	for (UINT i = 0; i < static_cast<UINT>(eBoundingSphereType::COUNT); ++i)
	{
		switch (i)
		{
		case static_cast<UINT>(eBoundingSphereType::ATTACK_SPHERE):
		{
			colliderRenderer.SphereRender(mSpheres[i], DirectX::Colors::GreenYellow);
			break;
		}
		case static_cast<UINT>(eBoundingSphereType::AGRO_SPHERE):
		{
			colliderRenderer.SphereRender(mSpheres[i], DirectX::Colors::FloralWhite);
			break;
		}
		default:
			break;
		}
	}

}


}


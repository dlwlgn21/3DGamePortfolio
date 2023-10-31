#include "Camera.h"
#include "D3DApp.h"
#include "GraphicDeviceDX11.h"
#include "GameObject.h"
#include "Transform.h"
#include "PlayerManager.h"
#include "DebugHelper.h"


using namespace DirectX::SimpleMath;

namespace jh
{
	Matrix Camera::sViewMatRow = Matrix::Identity;
	Matrix Camera::sProjectionMatRow = Matrix::Identity;


	Camera::Camera()
		: Component(jh::enums::eComponentType::CAMERA)
		, mViewMatRow(Matrix::Identity)
		, mProjectionMatRow(Matrix::Identity)
		, meProjectionType(eProjectionType::PERSPECTIVE)
		, mAspectRatio(D3DApp::GetInstance().GetWidth() / static_cast<float>(D3DApp::GetInstance().GetHeight()))
		, mFov(60.0f)
		, mNearPlain(0.1f)
		, mFarPlain(1000.0f)
		, mScale(1.0f)
	{
	}

	void Camera::FixedUpdate()
	{
		createViewMatrix();
		createProjectionMatrix();
		SetGpuViewMatrix(mViewMatRow);
		SetGpuProjectionMatrix(mProjectionMatRow);
	}

	void Camera::createViewMatrix()
	{
		auto& transform = GetOwner().GetTransform();
		Rotation& rot = transform.GetRotationRef();
		Vector3 targetPos = PlayerManager::GetInstance().GetPlayerTramsform().GetPosition() - transform.GetPosition();
		targetPos.y += 0.5f;
		mViewMatRow = DirectX::XMMatrixLookToLH(
			transform.GetPosition(),
			targetPos,
			transform.GetUpRef()
		);
		//mViewMatRow = Matrix::CreateRotationY(DirectX::XMConvertToRadians(-rot.YawDeg)) *
		//			  Matrix::CreateRotationX(DirectX::XMConvertToRadians(rot.PitchDeg)) *
		//			  Matrix::CreateTranslation(-transform.GetPositionRef());
		//mViewMatRow =	Matrix::CreateTranslation(-transform.GetPosition()) *
		//				Matrix::CreateRotationY(DirectX::XMConvertToRadians(-rot.YawDeg)) *
		//				Matrix::CreateRotationX(DirectX::XMConvertToRadians(rot.PitchDeg));
	}

	void Camera::createProjectionMatrix()
	{
		const D3D11_VIEWPORT* pViewPort = graphics::GraphicDeviceDX11::GetInstance().GetViewPort();
		switch (meProjectionType)
		{
		case Camera::PERSPECTIVE:
			mProjectionMatRow = DirectX::XMMatrixPerspectiveFovLH(
				DirectX::XMConvertToRadians(mFov),
				mAspectRatio,
				mNearPlain,
				mFarPlain
			);
			break;
		case Camera::ORTHOGRAPHIC:
			mProjectionMatRow = DirectX::XMMatrixOrthographicLH(
				pViewPort->Width,
				pViewPort->Height,
				mNearPlain,
				mFarPlain
			);
			break;
		default:
			assert(false);
			break;
		}
	}


}
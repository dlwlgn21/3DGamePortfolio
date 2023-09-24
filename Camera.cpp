#include "Camera.h"
#include "D3DApp.h"
#include "GraphicDeviceDX11.h"
#include "GameObject.h"
#include "Transform.h"

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
		, mNearPlain(1.0f)
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
		mViewMatRow = DirectX::XMMatrixLookAtLH(
			transform.GetPosition(),
			-transform.GetForward(),
			transform.GetUp()
		);
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
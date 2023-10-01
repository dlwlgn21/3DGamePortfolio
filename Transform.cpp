#include "Transform.h"
#include "GraphicsPSOManager.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;
using namespace jh::enums;

namespace jh
{
	const Vector3 Transform::S_FORWARD = Vector3(0.0f, 0.0f, 1.0f);
	const Vector3 Transform::S_RIGHT = Vector3(1.0f, 0.0f, 0.0f);
	const Vector3 Transform::S_UP = Vector3(0.0f, 1.0f, 0.0f);


Transform::Transform()
	: Component(eComponentType::TRANSFORM)
	, mForwardVector(S_FORWARD)
	, mRightVector(S_RIGHT)
	, mUpVector(S_UP)
	, mPosition(Vector3::Zero)
	, mRotation({0.0f, 0.0f, 0.0f})
	, mScale(Vector3::One)
	, mWolrdMatRow(Matrix::Identity)
	, mpParent(nullptr)
{
}

void Transform::FixedUpdate()
{

	// Make Modeling(World) Matrix
	// Rotation
	Matrix rotationMat = Matrix::Identity;
	rotationMat = Matrix::CreateRotationY(DirectX::XMConvertToRadians(mRotation.YawDeg));
	rotationMat *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(mRotation.PitchDeg));
	rotationMat *= Matrix::CreateRotationZ(DirectX::XMConvertToRadians(mRotation.RollDeg));
		
	mWolrdMatRow = Matrix::CreateScale(mScale) * rotationMat * Matrix::CreateTranslation(mPosition);
		 
	//mForwardVector = Vector3::TransformNormal(S_FORWARD, rotationMat);
	//mRightVector = Vector3::TransformNormal(S_RIGHT, rotationMat);
	//mUpVector = Vector3::TransformNormal(S_UP, rotationMat);
}

void Transform::UpdateConstantBuffer()
{
	auto& gpuBuffer = graphics::GraphicsPSOManager::GetInstance().GetConstantBuffer(graphics::eCBType::TRANSFORM);
	graphics::TransformConstantCPUBuffer cpuBuffer;
	ZeroMemory(&cpuBuffer, sizeof(graphics::TransformConstantCPUBuffer));
	cpuBuffer.WorldMat = mWolrdMatRow.Transpose();

	cpuBuffer.WolrdInvTransposedMat = cpuBuffer.WorldMat; 
	cpuBuffer.WolrdInvTransposedMat.Translation(Vector3(0.0f));
	cpuBuffer.WolrdInvTransposedMat = cpuBuffer.WolrdInvTransposedMat.Transpose().Invert();
	
	cpuBuffer.ViewMat = Camera::GetGpuViewMatrixRow().Transpose();
	cpuBuffer.ProjectionMat = Camera::GetGpuProjectionMatrixRow().Transpose();
	gpuBuffer.UpdateBuffer(&cpuBuffer);
}  



}

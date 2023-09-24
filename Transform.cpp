#include "Transform.h"
#include "GraphicsPSOManager.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;
using namespace jh::enums;

namespace jh
{

Transform::Transform()
	: Component(eComponentType::TRANSFORM)
	, mForwardVector(Vector3(0.0f, 0.0f, 1.0f))
	, mRightVector(Vector3(1.0f, 0.0f, 0.0f))
	, mUpVector(Vector3(0.0f, 1.0f, 0.0f))
	, mPosition(Vector3::Zero)
	, mRotation(Vector3::Zero)
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
	rotationMat = Matrix::CreateRotationY(DirectX::XMConvertToRadians(mRotation.y));
	rotationMat *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(mRotation.x));
	rotationMat *= Matrix::CreateRotationZ(DirectX::XMConvertToRadians(mRotation.z));
		
	mWolrdMatRow = Matrix::CreateScale(mScale) * rotationMat * Matrix::CreateTranslation(mPosition);
		 
	mForwardVector = Vector3::TransformNormal(Vector3::Forward, rotationMat);
	mRightVector = Vector3::TransformNormal(Vector3::Right, rotationMat);
	mUpVector = Vector3::TransformNormal(Vector3::Up, rotationMat);
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

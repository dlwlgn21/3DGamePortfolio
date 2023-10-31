#include "Transform.h"
#include "GraphicsPSOManager.h"
#include "Camera.h"
#include "LightingManager.h"

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
	, mRotation({ 0.0f, 0.0f, 0.0f })
	, mQuaternion(Vector4(0.0f, 0.0f, 0.0f, 0.0f))
	, mScale(Vector3::One)
	, mWolrdMatRow(Matrix::Identity)
	, mpParent(nullptr)
{
}

void Transform::FixedUpdate()
{

	// Make Modeling(World) Matrix
	// Rotation
	mQuaternion = Quaternion::CreateFromYawPitchRoll(
		Vector3(
			DirectX::XMConvertToRadians(mRotation.PitchDeg),
			DirectX::XMConvertToRadians(mRotation.YawDeg),
			DirectX::XMConvertToRadians(mRotation.RollDeg)
		)
	);
	Matrix rMat = Matrix::CreateFromQuaternion(mQuaternion);
	mWolrdMatRow = Matrix::CreateScale(mScale) * rMat * Matrix::CreateTranslation(mPosition);

	mForwardVector = Vector3::TransformNormal(S_FORWARD, rMat);
	mRightVector = Vector3::TransformNormal(S_RIGHT, rMat);
	mUpVector = Vector3::TransformNormal(S_UP, rMat);
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

void Transform::UpdateShadowConstantBuffer()
{
	auto& gpuBuffer = graphics::GraphicsPSOManager::GetInstance().GetConstantBuffer(graphics::eCBType::TRANSFORM);
	auto& lightingManager = LightingManager::GetInstance();
	graphics::TransformConstantCPUBuffer cpuBuffer;
	ZeroMemory(&cpuBuffer, sizeof(graphics::TransformConstantCPUBuffer));
	cpuBuffer.WorldMat = mWolrdMatRow.Transpose();

	cpuBuffer.WolrdInvTransposedMat = cpuBuffer.WorldMat;
	cpuBuffer.WolrdInvTransposedMat.Translation(Vector3(0.0f));
	cpuBuffer.WolrdInvTransposedMat = cpuBuffer.WolrdInvTransposedMat.Transpose().Invert();

	cpuBuffer.ViewMat = lightingManager.GetShadowMappingViewMatrixRow().Transpose();
	cpuBuffer.ProjectionMat = lightingManager.GetShadowMappingProjectionMatrixRow().Transpose();
	gpuBuffer.UpdateBuffer(&cpuBuffer);
}

}

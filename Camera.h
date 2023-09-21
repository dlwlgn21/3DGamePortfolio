#pragma once
#include "Component.h"

namespace jh
{


class Camera final : public Component
{
public:
	enum eProjectionType
	{
		PERSPECTIVE,
		ORTHOGRAPHIC,
		COUNT,
	};

	__forceinline static DirectX::SimpleMath::Matrix GetGpuViewMatrixRow() { return sViewMatRow; }
	__forceinline static DirectX::SimpleMath::Matrix GetGpuProjectionMatrixRow() { return sProjectionMatRow; }
	__forceinline static void SetGpuViewMatrix(const DirectX::SimpleMath::Matrix& viewMatRow) { sViewMatRow = viewMatRow; }
	__forceinline static void SetGpuProjectionMatrix(const DirectX::SimpleMath::Matrix& projectionMatRow) { sProjectionMatRow = projectionMatRow; }

	Camera();
	virtual ~Camera() = default;

	void FixedUpdate() override;


	void SetProjectionType(const eProjectionType type) { meProjectionType = type; }
	float GetScale() { return mScale; }
	DirectX::SimpleMath::Matrix& GetViewMatrixRow() { return mViewMatRow; }
	DirectX::SimpleMath::Matrix& GetProjectionMatrixRow() { return mProjectionMatRow; }

private:
	void createViewMatrix();
	void createProjectionMatrix();

private:
	static DirectX::SimpleMath::Matrix sViewMatRow;
	static DirectX::SimpleMath::Matrix sProjectionMatRow;

	DirectX::SimpleMath::Matrix mViewMatRow;
	DirectX::SimpleMath::Matrix mProjectionMatRow;

	eProjectionType meProjectionType;
	float mAspectRatio;
	float mFov;
	float mNearPlain;
	float mFarPlain;
	float mScale;
};


}


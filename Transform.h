#pragma once
#include "Component.h"

namespace jh
{
	class Transform final : public Component
	{
	public:
		Transform();
		virtual ~Transform() = default;

		void FixedUpdate() override;

		__forceinline void SetPosition(const DirectX::SimpleMath::Vector3 pos) { mPosition = pos; }
		__forceinline void SetRotation(const DirectX::SimpleMath::Vector3 rotation) { mRotation = rotation; }
		__forceinline void SetScale(const DirectX::SimpleMath::Vector3 scale) { mScale = scale; }
		__forceinline void Set2DPosition(const float x, const float y) { mPosition.x = x; mPosition.y = y; }
		__forceinline void Set2DPosition(const DirectX::SimpleMath::Vector2 pos) { mPosition.x = pos.x; mPosition.y = pos.y; }
		__forceinline void Set2DRotation(const float angle) { mRotation.z = angle; }
		
		//void SetParent(GameObject* pGameObject) { assert(pGameObject != nullptr); mpParent = pGameObject->GetTransform(); }

		__forceinline DirectX::SimpleMath::Vector3 GetPosition() const { return mPosition; }
		__forceinline DirectX::SimpleMath::Vector3 GetRotation() const { return mRotation; }
		__forceinline DirectX::SimpleMath::Vector3 GetScale()	 const { return mScale; }

		__forceinline DirectX::SimpleMath::Vector3& GetPositionRef() { return mPosition; }
		__forceinline DirectX::SimpleMath::Vector3& GetRotationRef() { return mRotation; }
		__forceinline DirectX::SimpleMath::Vector3& GetScaleRef()	 { return mScale; }

		__forceinline float GetOnlyXPosition() const { return mPosition.x; }
		__forceinline float GetOnlyYPosition() const { return mPosition.y; }
		__forceinline DirectX::SimpleMath::Vector2 GetOnlyXYPosition() const { return DirectX::SimpleMath::Vector2(mPosition.x, mPosition.y); }
		__forceinline float GetOnlyXScale()	const { return mScale.x; }

		__forceinline const DirectX::SimpleMath::Vector3& GetForward() const { return mForwardVector; }
		__forceinline const DirectX::SimpleMath::Vector3& GetRight() const	{ return mRightVector; }
		__forceinline const DirectX::SimpleMath::Vector3& GetUp() const	{ return mUpVector; }
		const DirectX::SimpleMath::Matrix& GetWorldMatrix()			{ return mWolrdMatRow; }

		void UpdateConstantBuffer();

	private:
		DirectX::SimpleMath::Vector3		mForwardVector;
		DirectX::SimpleMath::Vector3		mRightVector;
		DirectX::SimpleMath::Vector3		mUpVector;

		DirectX::SimpleMath::Vector3		mPosition;
		DirectX::SimpleMath::Vector3		mRotation;
		DirectX::SimpleMath::Vector3		mScale;

		DirectX::SimpleMath::Matrix			mWolrdMatRow;
		Transform*							mpParent;
	};
}



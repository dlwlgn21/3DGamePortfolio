#pragma once
#include "Component.h"

namespace jh
{

struct Rotation
{
	float YawDeg = 0.0f;
	float RollDeg = 0.0f;
	float PitchDeg = 0.0f;

	__forceinline void Clamp()
	{
		YawDeg = GetAxisClampedValue(YawDeg);
		RollDeg = GetAxisClampedValue(RollDeg);
		PitchDeg = GetAxisClampedValue(PitchDeg);
	}

	__forceinline float GetAxisClampedValue(float deg)
	{
		float angle = std::fmodf(deg, 360.f);
		if (angle < 0.f)
		{
			angle += 360.f;
		}

		return angle;
	}
};

class Transform final : public Component
{
public:
	Transform();
	virtual ~Transform() = default;

	void FixedUpdate() override;

	__forceinline void SetPosition(const DirectX::SimpleMath::Vector3 pos)	{ mPosition = pos; }
	__forceinline void SetRotation(const Rotation rotation)					{ mRotation = rotation; }
	__forceinline void SetYaw(const float yawDeg)							{ mRotation.YawDeg = yawDeg; }
	__forceinline void SetRoll(const float rollDeg)							{ mRotation.RollDeg = rollDeg; }
	__forceinline void SetPitch(const float pitchDeg)						{ mRotation.PitchDeg = pitchDeg; }
	__forceinline void AccumulateYaw(const float yawDeg)
	{
		mRotation.YawDeg += yawDeg;
		mRotation.Clamp();
	}
	__forceinline void AccumulateRoll(const float rollDeg)
	{ 
		mRotation.RollDeg += rollDeg; 
		mRotation.Clamp();
	}
	__forceinline void AccumulatePitch(const float pitchDeg)
	{ 
		mRotation.PitchDeg += pitchDeg; 
		mRotation.Clamp();
	}
	__forceinline void SetScale(const DirectX::SimpleMath::Vector3 scale)	{ mScale = scale; }

		
	//void SetParent(GameObject* pGameObject) { assert(pGameObject != nullptr); mpParent = pGameObject->GetTransform(); }

	__forceinline DirectX::SimpleMath::Vector3  GetPosition() const	{ return mPosition; }
	__forceinline Rotation					    GetRotation() const	{ return mRotation; }
	__forceinline DirectX::SimpleMath::Vector3  GetScale()	 const	{ return mScale; }

	__forceinline DirectX::SimpleMath::Vector3& GetPositionRef()	{ return mPosition; }
	__forceinline Rotation&                     GetRotationRef()	{ return mRotation; }
	__forceinline DirectX::SimpleMath::Vector3& GetScaleRef()		{ return mScale; }


	__forceinline float GetYawDeg()		const 		{ return mRotation.YawDeg;		}
	__forceinline float GetRollDeg()	const 		{ return mRotation.RollDeg;		}
	__forceinline float GetPitchDeg()	const		{ return mRotation.PitchDeg;	}
	__forceinline float& GetYawDegRef()				{ return mRotation.YawDeg;		}
	__forceinline float& GetRollDegRef()			{ return mRotation.RollDeg;		}
	__forceinline float& GetPitchDegRef()			{ return mRotation.PitchDeg;	}

	__forceinline DirectX::SimpleMath::Vector3& GetForwardRef()		{ return mForwardVector; }
	__forceinline DirectX::SimpleMath::Vector3& GetRightRef()		{ return mRightVector; }
	__forceinline DirectX::SimpleMath::Vector3& GetUpRef()			{ return mUpVector; }
	const DirectX::SimpleMath::Matrix& GetWorldMatrixRow()			{ return mWolrdMatRow; }
	void UpdateConstantBuffer();

public:
	static const DirectX::SimpleMath::Vector3 S_FORWARD;
	static const DirectX::SimpleMath::Vector3 S_RIGHT;
	static const DirectX::SimpleMath::Vector3 S_UP;

private:
	DirectX::SimpleMath::Vector3		mForwardVector;
	DirectX::SimpleMath::Vector3		mRightVector;
	DirectX::SimpleMath::Vector3		mUpVector;

	DirectX::SimpleMath::Vector3		mPosition;
	Rotation							mRotation;
	DirectX::SimpleMath::Quaternion		mQuaternion;
	DirectX::SimpleMath::Vector3		mScale;

	DirectX::SimpleMath::Matrix			mWolrdMatRow;
	Transform*							mpParent;
};
}



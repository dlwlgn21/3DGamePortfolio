#include "SkinnedMeshRenderer.h"
#include "GameObject.h"
#include "Transform.h"

#include "InputManager.h"
#include "Time.h"
#include "DebugHelper.h"

#include "GraphicDeviceDX11.h"
#include "SkinnedMeshModel.h"
#include "AnimationDataManager.h"


using namespace jh::enums;
using namespace jh::graphics;

namespace jh
{
static int frameCount = 0;

SkinnedMeshRenderer::SkinnedMeshRenderer()
	: Renderer(eRenererComponentType::SKINNED_MESH_RENDERER)
{
	mClipKeys.reserve(static_cast<UINT>(eCharacterAnimState::COUNT));
	auto& clipKeys = AnimationDataManager::GetInstance().GetAnimationCilpKeys(eAnimClipKeyContainerType::PLAYER);
	for (const auto& key : clipKeys)
	{
		mClipKeys.push_back(key);
	}
}



void SkinnedMeshRenderer::Update()
{
	SkinnedMeshModel* pModel = static_cast<SkinnedMeshModel*>(mpModel);
	jh::graphics::AnimationData& animData = pModel->GetAnimData();
	UINT clipIndex = static_cast<UINT>(meState);
	switch (meState)
	{
	case eCharacterAnimState::IDLE:
	{
		if (InputManager::GetKeyState(eKeyCode::UP) == eKeyState::PRESSED)
		{
			meState = eCharacterAnimState::IDLE_TO_WALK;
			frameCount = 0;
		}
		else if (animData.ClipMap[mClipKeys[clipIndex]].IsLastFrame(frameCount))
		{
			frameCount = 0;
		}
		break;
	}
	case eCharacterAnimState::IDLE_TO_WALK:
	{
		if (animData.ClipMap[mClipKeys[clipIndex]].IsLastFrame(frameCount))
		{
			meState = eCharacterAnimState::WALK_FORWARD;
			frameCount = 0;
		}
		break;
	}
	case eCharacterAnimState::WALK_FORWARD:
	{
		auto& transform = GetOwner().GetTransform();
		Vector3 moveVector = transform.GetPosition();
		if (InputManager::GetKeyState(eKeyCode::RIGHT) == eKeyState::PRESSED)
		{
			transform.AccumulateYaw(120.0f * Time::DeltaTime());
			//animData.AccumulatedRootTransformMatrix =
			//	Matrix::CreateRotationY(DirectX::XMConvertToRadians(60.0f) * Time::DeltaTime()) *
			//	animData.AccumulatedRootTransformMatrix;

		}
		if (InputManager::GetKeyState(eKeyCode::LEFT) == eKeyState::PRESSED)
		{
			transform.AccumulateYaw(-120.0f * Time::DeltaTime());
			//animData.AccumulatedRootTransformMatrix =
			//	Matrix::CreateRotationY(-DirectX::XMConvertToRadians(60.0f) * Time::DeltaTime()) *
			//	animData.AccumulatedRootTransformMatrix;
		}

		moveVector += -transform.GetForwardRef() * 4.0f * Time::DeltaTime();
		transform.SetPosition(moveVector);
		if (animData.ClipMap[mClipKeys[clipIndex]].IsLastFrame(frameCount))
		{
			if (InputManager::GetKeyState(eKeyCode::UP) != eKeyState::PRESSED)
			{
				meState = eCharacterAnimState::WALK_TO_IDLE;
			}
			frameCount = 0;
		}
		break;
	}
	case eCharacterAnimState::WALK_TO_IDLE:
	{
		if (animData.ClipMap[mClipKeys[clipIndex]].IsLastFrame(frameCount))
		{
			meState = eCharacterAnimState::IDLE;
			frameCount = 0;
		}
		break;
	}
	default:
		assert(false);
		break;
	}
	++frameCount;
}

void SkinnedMeshRenderer::Render()
{
	UINT clipIndex = static_cast<UINT>(meState);
	assert(mpModel != nullptr);
	GetOwner().GetTransform().UpdateConstantBuffer(); 
	static_cast<SkinnedMeshModel*>(mpModel)->Render(mClipKeys[clipIndex], frameCount);
}


}
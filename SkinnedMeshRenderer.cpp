#include "SkinnedMeshRenderer.h"
#include "GameObject.h"
#include "Transform.h"


#include "InputManager.h"
#include "Time.h"

#include "GraphicDeviceDX11.h"
#include "SkinnedMeshModel.h"


using namespace jh::enums;
using namespace jh::graphics;

namespace jh
{
static int frameCount = 0;

SkinnedMeshRenderer::SkinnedMeshRenderer()
	: Renderer(eRenererComponentType::SKINNED_MESH_RENDERER)
{

}

void SkinnedMeshRenderer::Update()
{
	SkinnedMeshModel* pModel = static_cast<SkinnedMeshModel*>(mpModel);
	jh::graphics::AnimationData& animData = pModel->GetAnimData();
	switch (meState)
	{
	case eCharacterAnimState::IDLE:
	{
		if (InputManager::GetKeyState(eKeyCode::UP) == eKeyState::PRESSED)
		{
			meState = eCharacterAnimState::IDLE_TO_WALK;
			frameCount = 0;
		}
		else if (frameCount == animData.ClipArray[static_cast<UINT>(meState)].KeyBoneAndFrame2DArrays[0].size())
		{
			frameCount = 0;
		}
		break;
	}
	case eCharacterAnimState::IDLE_TO_WALK:
	{
		if (frameCount == animData.ClipArray[static_cast<UINT>(meState)].KeyBoneAndFrame2DArrays[0].size())
		{
			meState = eCharacterAnimState::WALK_FORWARD;
			frameCount = 0;
		}
		break;
	}
	case eCharacterAnimState::WALK_FORWARD:
	{
		if (InputManager::GetKeyState(eKeyCode::RIGHT) == eKeyState::PRESSED)
		{
			animData.AccumulatedRootTransformMatrix =
				Matrix::CreateRotationY(DirectX::XMConvertToRadians(60.0f) * Time::DeltaTime()) *
				animData.AccumulatedRootTransformMatrix;
		}
		if (InputManager::GetKeyState(eKeyCode::LEFT) == eKeyState::PRESSED)
		{
			animData.AccumulatedRootTransformMatrix =
				Matrix::CreateRotationY(-DirectX::XMConvertToRadians(60.0f) * Time::DeltaTime()) *
				animData.AccumulatedRootTransformMatrix;
		}
		if (frameCount == animData.ClipArray[static_cast<UINT>(meState)].KeyBoneAndFrame2DArrays[0].size())
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
		if (frameCount == animData.ClipArray[static_cast<UINT>(meState)].KeyBoneAndFrame2DArrays[0].size())
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
	assert(mpModel != nullptr);
	GetOwner().GetTransform().UpdateConstantBuffer();
	static_cast<SkinnedMeshModel*>(mpModel)->Render(static_cast<UINT>(meState), frameCount);
}


}
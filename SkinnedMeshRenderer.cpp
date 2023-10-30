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
}

void SkinnedMeshRenderer::Render()
{
	GetOwner().GetTransform().UpdateConstantBuffer(); 
	SetIBLShaderResourceViews();
	mpModel->Render();
}


}
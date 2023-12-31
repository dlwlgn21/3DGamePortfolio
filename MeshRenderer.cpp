#include "MeshRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "GraphicDeviceDX11.h"


using namespace jh::enums;
using namespace jh::graphics;

namespace jh
{

MeshRenderer::MeshRenderer()
	: Renderer(eRenererComponentType::MESH_RENDERER)
{
}

}
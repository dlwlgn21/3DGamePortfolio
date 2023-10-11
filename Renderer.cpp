#include "Renderer.h"

using namespace jh::enums;

namespace jh
{


Renderer::Renderer(const jh::enums::eRenererComponentType eRendererType)
	: Component(eComponentType::RENDERER)
	, meRendererType(eRendererType)
{
}

}
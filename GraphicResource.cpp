#include "GraphicResource.h"

namespace jh::graphics
{
	GraphicResource::GraphicResource(const jh::enums::eResourceType eType)
		: meType(eType)
	{
	}
	HRESULT GraphicResource::Load(const std::wstring& path)
	{
		return S_OK;
	}
}
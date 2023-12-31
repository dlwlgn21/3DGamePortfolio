#pragma once
#include <vector>
#include "GraphicsDatas.h"

namespace jh::graphics
{
	struct MeshData
	{
		std::vector<Vertex3D>		Vertices;
		std::vector<SkinnedVertex>	SkinnedVertices;
		std::vector<UINT>			Indices;
		std::wstring				DiffuseTextureFileFullPath;
		std::wstring				NormalTextureFileFullPath;
	};
}



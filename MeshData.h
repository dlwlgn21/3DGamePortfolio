#pragma once
#include <vector>
#include "GraphicsDatas.h"
#include "Texture.h"

namespace jh::graphics
{
	struct MeshData
	{
		std::vector<Vertex3D>	Vertices;
		std::vector<UINT>		Indices;
		std::string				TextureFileFullPath;
	};
}



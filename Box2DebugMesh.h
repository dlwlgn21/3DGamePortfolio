#pragma once
#include "Mesh.h"
#include "GraphicsDatas.h"
namespace jh::graphics
{
class Box2DebugMesh final : public Mesh
{
public:
	Box2DebugMesh();
	virtual ~Box2DebugMesh() = default;

	void UpdateVertexBuffer(std::vector<Box2DVertex>& vertices);
};


}


#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Material.h"

namespace jh
{


class Mesh;
class Renderer : public Component
{
public:
	Renderer(const jh::enums::eRenererComponentType eRendererType);
	virtual ~Renderer() = default;

	jh::enums::eRenererComponentType GetType() const { return meRendererType; }

	void SetMaterial(jh::graphics::Material* pMaterial) { assert(pMaterial != nullptr); mpMaterial = pMaterial; };
	void SetMesh(jh::graphics::Mesh* pMesh) { assert(pMesh != nullptr); mpMesh = pMesh; }

protected:
	const jh::enums::eRenererComponentType	meRendererType;
	jh::graphics::Mesh*						mpMesh;
	jh::graphics::Material*					mpMaterial;
};


}
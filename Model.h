#pragma once
#include "GraphicResource.h"

namespace jh::graphics
{

class Mesh;
class Material;
class Model : public GraphicResource
{
public:
	Model();
	virtual ~Model() = default;

	void InitMeshes(std::vector<Mesh*>& pMeshes);
	void InitMaterials(std::vector<Material*>& pMaterials);

	void Render();
private:
	std::vector<jh::graphics::Mesh*> mpMeshes;
	std::vector<jh::graphics::Material*> mpMaterials;
};

}


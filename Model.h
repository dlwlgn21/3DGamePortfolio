#pragma once
#include "GraphicResource.h"

namespace jh::graphics
{

enum class eModelType
{
	NON_SKINNED_MESH_MODEL,
	SKINNED_MESH_MODEL,
	COUNT
};

class Mesh;
class Material;
class Model : public GraphicResource
{
public:
	Model();
	virtual ~Model() = default;

	void SetModelType(const eModelType eType) { meType = eType; }
	const eModelType GetType() const		  {return meType;}

	void InitMeshes(std::vector<Mesh*>& pMeshes);
	void InitMaterials(std::vector<Material*>& pMaterials);

	void Render();
private:
	std::vector<jh::graphics::Mesh*> mpMeshes;
	std::vector<jh::graphics::Material*> mpMaterials;
	eModelType meType = eModelType::NON_SKINNED_MESH_MODEL;
};

}


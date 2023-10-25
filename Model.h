#pragma once
#include "GraphicResource.h"
#include "BoneAnimator.h"

namespace jh::graphics
{

enum class eModelType
{
	NON_SKINNED_MESH_MODEL,
	SKINNED_MESH_MODEL,
	DEBUG_COLLIDER_MODEL,
	COUNT
};

class Mesh;
class Material;
class Model : public GraphicResource
{
public:
	Model();
	virtual ~Model() = default;

	void SetModelType(const eModelType eType) { meModelType = eType; }
	const eModelType GetType() const		  {return meModelType;}

	void SetMeshes(std::vector<Mesh*>& pMeshes);
	void SetMaterials(std::vector<Material*>& pMaterials);
	void SetBoneAnimator(jh::BoneAnimator& boneAnimator);
	void SetPipeline();
	void Render();
private:
	std::vector<jh::graphics::Mesh*>			mpMeshes;
	std::vector<jh::graphics::Material*>		mpMaterials;
	jh::BoneAnimator*							mpBoneAnimator;
	eModelType									meModelType;
};	

}


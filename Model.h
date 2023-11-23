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
	const eModelType GetType() const { return meModelType; }

	void SetMeshes(std::vector<Mesh*>& pMeshes);
	void SetMaterial(jh::graphics::Material& material);
	void SetBoneAnimator(jh::BoneAnimator& boneAnimator);
	//void SetPipeline();

	virtual void ShadowRender();
	virtual void Render();

protected:
	std::vector<jh::graphics::Mesh*>			mpMeshes;
	jh::graphics::Material*						mpMaterial;
	jh::BoneAnimator*							mpBoneAnimator;
	eModelType									meModelType;
};

}
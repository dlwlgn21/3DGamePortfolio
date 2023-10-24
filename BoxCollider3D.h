#include "Component.h"
#include "Model.h"
//#include <directxtk/PrimitiveBatch.h>
//#include <directxtk/VertexTypes.h>
namespace jh
{
using namespace DirectX;

class BoxCollider3D final : public Component
{
public:
	BoxCollider3D();
	virtual ~BoxCollider3D() = default;

	void InitBoxScale(const DirectX::SimpleMath::Vector3& scale) { mBoxScale = scale; }
	void SetModel(jh::graphics::Model* pModel) { assert(pModel != nullptr); mpDebugDrawModel = pModel; }
	void Render();

private:
	jh::graphics::Model*													mpDebugDrawModel;
	DirectX::SimpleMath::Vector3											mBoxScale;
	//DirectX::BoundingBox													mBoundingBox;
	//std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>	mspPrimitiveBatch;
};

}


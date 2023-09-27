#pragma once
#include "Common.h"
#include "Material.h"
#include "Mesh.h"

namespace jh
{

class CubeMapManager final
{
public:
	static CubeMapManager& GetInstance()
	{
		static CubeMapManager instance;
		return instance;
	}

	CubeMapManager(const CubeMapManager& other) = delete;
	CubeMapManager& operator=(const CubeMapManager& other) = delete;

	void Initialize();
	void Render();

private:
	CubeMapManager() = default;
	~CubeMapManager() = default;

private:
	jh::graphics::Material* mpMaterial = nullptr;
	jh::graphics::Mesh* mpMesh = nullptr;
};

}



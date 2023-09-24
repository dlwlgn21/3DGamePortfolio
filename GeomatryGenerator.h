#pragma once
#include "Common.h"
#include "GraphicsDatas.h"
#include "MeshData.h"

namespace jh::graphics
{
	struct MeshData;
	class GeomatryGenerator final
	{
	public:
		static GeomatryGenerator& GetInstance()
		{
			static GeomatryGenerator instance;
			return instance;
		}
		GeomatryGenerator(const GeomatryGenerator& other) = delete;
		GeomatryGenerator& operator=(const GeomatryGenerator& other) = delete;

		std::vector<Vertex2D> GetTriangle();
		std::vector<Vertex2D> GetSquare();
		void MakeBox(std::vector<Vertex3D>& outVertices, std::vector<UINT>& outIndices);
		void MakeSphere(std::vector<Vertex3D>& outVertices, std::vector<UINT>& outIndices, const float radius, const int numSlices, const int numStacks);

		std::vector<MeshData> ReadFromFile(const std::string& basePath, const std::string& fileName);


	private:
		GeomatryGenerator() = default;
		~GeomatryGenerator() = default;

	};


}

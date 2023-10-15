#pragma once
#include "Common.h"
#include "GraphicsDatas.h"
#include "MeshData.h"
#include "AnimationData.h"

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
		void MakeWorldCoordinate(std::vector<Vertex3D>& outVertices, std::vector<UINT>& outIndices, const float scale = 1.0f);
		void MakeBox(std::vector<Vertex3D>& outVertices, std::vector<UINT>& outIndices, const float scale = 1.0f);
		void MakeSphere(std::vector<Vertex3D>& outVertices, std::vector<UINT>& outIndices, const float radius, const int numSlices, const int numStacks);

		std::vector<MeshData> ReadFromFile(const std::string& basePath, const std::string& fileName);

		std::tuple<std::vector<MeshData>, AnimationData*> ReadFBXFile(const std::string& basePath, const std::string& filename, const std::string& animKey, const bool bIsRevertNormals = false);
		void ParseAnimationClip(const std::string& basePath, const std::string& filename, const std::string& animKey, const std::string& clipKey);

	private:
		void normalize(const Vector3 center, const float longestLength, std::vector<MeshData>& meshes, AnimationData& animData);

	private:
		GeomatryGenerator() = default;
		~GeomatryGenerator() = default;

	};


}

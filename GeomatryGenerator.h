#pragma once
#include "Common.h"
#include "GraphicsDatas.h"

namespace jh::graphics
{
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

	private:
		GeomatryGenerator() = default;
		~GeomatryGenerator() = default;

	};


}

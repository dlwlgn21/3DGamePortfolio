#include "GeomatryGenerator.h"

using namespace DirectX::SimpleMath;

namespace jh::graphics
{
	std::vector<Vertex2D> GeomatryGenerator::GetTriangle()
	{
		std::vector<Vertex2D> triangle;

		triangle.reserve(3);
		triangle.resize(triangle.capacity());

		triangle[0].Pos = Vector3(0.0f, 0.5f, 0.5f);
		triangle[0].UV = Vector2(0.0f, 0.0f);

		triangle[1].Pos = Vector3(0.5f, -0.5f, 0.5f);
		triangle[1].UV = Vector2(0.0f, 0.0f);

		triangle[2].Pos = Vector3(-0.5f, -0.5f, 0.5f);
		triangle[2].UV = Vector2(0.0f, 0.0f);

		return triangle;
	}
	std::vector<Vertex2D> GeomatryGenerator::GetSquare()
	{
		std::vector<Vertex2D> squre;
		squre.reserve(4);
		squre.resize(squre.capacity());

		squre[0].Pos = Vector3(-0.5f, 0.5f, 0.0f);
		squre[0].UV = Vector2(0.0f, 0.0f);

		squre[1].Pos = Vector3(0.5f, 0.5f, 0.0f);
		squre[1].UV = Vector2(1.0f, 0.0f);

		squre[2].Pos = Vector3(0.5f, -0.5f, 0.0f);
		squre[2].UV = Vector2(1.0f, 1.0f);

		squre[3].Pos = Vector3(-0.5f, -0.5f, 0.0f);
		squre[3].UV = Vector2(0.0f, 1.0f);

		return squre;
	}
}
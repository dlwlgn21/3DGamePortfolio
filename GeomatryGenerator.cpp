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

void GeomatryGenerator::MakeBox(std::vector<Vertex3D>& outVertices, std::vector<UINT>& outIndices)
{
    outVertices.reserve(24);
    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> texcoords;

    // 윗면
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f));
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // 아랫면
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) );
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) );
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) );
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // 앞면
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) );
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) );
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) );
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // 뒷면
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) );
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) );
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) );
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // 왼쪽
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) );
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) );
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) );
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // 오른쪽
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f));
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    for (size_t i = 0; i < positions.size(); i++) {
        Vertex3D v;
        v.Pos = positions[i];
        v.Normal = normals[i];
        v.UV = texcoords[i];
        outVertices.push_back(v);
    }
    outIndices.reserve(36);
    outIndices = 
    {
        0,  1,  2,  0,  2,  3,  // 윗면
        4,  5,  6,  4,  6,  7,  // 아랫면
        8,  9,  10, 8,  10, 11, // 앞면
        12, 13, 14, 12, 14, 15, // 뒷면
        16, 17, 18, 16, 18, 19, // 왼쪽
        20, 21, 22, 20, 22, 23  // 오른쪽
    };

}

void GeomatryGenerator::MakeSphere(std::vector<Vertex3D>& outVertices, std::vector<UINT>& outIndices, const float radius, const int numSlices, const int numStacks)
{
    const float dTheta = -DirectX::XM_2PI / float(numSlices);
    const float dPhi = -DirectX::XM_PI / float(numStacks);

    outVertices.reserve(10000);
    for (int j = 0; j <= numStacks; j++) 
    {
        // 스택에 쌓일 수록 시작점을 x-y 평면에서 회전 시켜서 위로 올리는 구조
        Vector3 stackStartPoint = Vector3::Transform(
            Vector3(0.0f, -radius, 0.0f), 
            Matrix::CreateRotationZ(dPhi * j)
        );

        for (int i = 0; i <= numSlices; i++) 
        {
            Vertex3D v;
            // 시작점을 x-z 평면에서 회전시키면서 원을 만드는 구조
            v.Pos = Vector3::Transform(
                stackStartPoint, 
                Matrix::CreateRotationY(dTheta * float(i))
            );

            v.Normal = v.Pos; // 원점이 구의 중심
            v.Normal.Normalize();
            v.UV = Vector2(float(i) / numSlices, 1.0f - float(j) / numStacks);
            outVertices.push_back(v);
        }
    }
    outIndices.reserve(1000);
    for (int j = 0; j < numStacks; j++) 
    {
        const int offset = (numSlices + 1) * j;
        for (int i = 0; i < numSlices; i++) 
        {

            outIndices.push_back(offset + i);
            outIndices.push_back(offset + i + numSlices + 1);
            outIndices.push_back(offset + i + 1 + numSlices + 1);

            outIndices.push_back(offset + i);
            outIndices.push_back(offset + i + 1 + numSlices + 1);
            outIndices.push_back(offset + i + 1);
        }
    }
}

}
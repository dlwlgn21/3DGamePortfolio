#include "GeomatryGenerator.h"
#include "ModelLoader.h"
#include "AnimationData.h"

using namespace DirectX;
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

void GeomatryGenerator::MakeSquare(std::vector<Vertex3D>& outVertices, std::vector<UINT>& outIndices, const float scale)
{
    outVertices.reserve(24);
    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> texcoords;
    positions.reserve(outVertices.capacity());
    normals.reserve(outVertices.capacity());
    texcoords.reserve(outVertices.capacity());

    // 윗면
    positions.push_back(Vector3(-1.0f, 0.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 0.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 0.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 0.0f, -1.0f) * scale);
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    for (UINT i = 0; i < positions.size(); i++)
    {
        Vertex3D v;
        v.Pos = positions[i];
        v.Normal = normals[i];
        v.UV = texcoords[i];
        v.TangentModel = Vector3(1.0f, 0.0f, 0.0f);
        outVertices.push_back(v);
    }

    outIndices.reserve(6);
    outIndices = { 0,  1,  2,  0,  2,  3 };
}


void GeomatryGenerator::MakeWorldCoordinate(std::vector<Vertex3D>& outVertices, std::vector<UINT>& outIndices, const float scale)
{
    std::vector<Vector3> positions;
    std::vector<Vector3> colors;
    std::vector<Vector2> uvs;

    // X 축 
    positions.push_back(Vector3(0.0f, 0.0f, 0.0f));
    positions.push_back(Vector3(1.0f, 0.0f, 0.0f) * scale);
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    uvs.push_back(Vector2(0.0f, 0.0f));
    uvs.push_back(Vector2(1.0f, 1.0f));
    // Y 축
    positions.push_back(Vector3(0.0f, 0.0f, 0.0f));
    positions.push_back(Vector3(0.0f, 1.0f, 0.0f) * scale);
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    uvs.push_back(Vector2(0.0f, 0.0f));
    uvs.push_back(Vector2(1.0f, 1.0f));

    // Z 축
    positions.push_back(Vector3(0.0f, 0.0f, 0.0f));
    positions.push_back(Vector3(0.0f, 0.0f, 1.0f) * scale);
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    uvs.push_back(Vector2(0.0f, 0.0f));
    uvs.push_back(Vector2(1.0f, 1.0f));


    for (UINT i = 0; i < 6; ++i)
    {
        Vertex3D v;
        v.Pos = positions[i];
        v.Normal = colors[i];
        v.UV = uvs[i];
        outVertices.push_back(v);
    }
    for (UINT i = 0; i < positions.size() / 2; ++i)
    {
        outIndices.push_back(2 * i);
        outIndices.push_back(2 * i + 1);
    }
}

void GeomatryGenerator::MakeBox(std::vector<Vertex3D>& outVertices, std::vector<UINT>& outIndices, const float scale)
{
    outVertices.reserve(24);
    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> texcoords;
    positions.reserve(outVertices.capacity());
    normals.reserve(outVertices.capacity());
    texcoords.reserve(outVertices.capacity());

    // 윗면
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // 아랫면
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // 앞면
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // 뒷면
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // 왼쪽
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // 오른쪽
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    for (size_t i = 0; i < positions.size(); i++) 
    {
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

void GeomatryGenerator::MakeDebugDrawingColliderBox(std::vector<Vertex3DCollider>& outVertices, std::vector<UINT>& outIndices)
{
    outVertices.reserve(24);
    std::vector<Vector3> positions;
    positions.reserve(outVertices.capacity());

    // 윗면
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f));
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f));

    // 아랫면
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f));
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f));

    // 앞면
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f));
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f));
    // 뒷면
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f));
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f));
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f));

    // 왼쪽
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f));
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f));
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f));
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f));

    // 오른쪽
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f));
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f));

    for (size_t i = 0; i < positions.size(); i++) 
    {
        Vertex3DCollider v;
        v.Pos = positions[i];
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


            Vector3 biTangent = Vector3(0.0f, 1.0f, 0.0f);

            Vector3 normalOrth =
                v.Normal - biTangent.Dot(v.Normal) * v.Normal;
            normalOrth.Normalize();

            v.TangentModel = biTangent.Cross(normalOrth);
            v.TangentModel.Normalize();

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

std::vector<MeshData> GeomatryGenerator::ReadFromFile(const std::string& basePath, const std::string& fileName)
{
    ModelLoader loader;
    loader.Load(basePath, fileName);
    std::vector<MeshData>& meshDatas = loader.MeshDatas;
    Vector3 vmin(1000, 1000, 1000);
    Vector3 vmax(-1000, -1000, -1000);
    for (MeshData& mesh : meshDatas) 
    {
        for (Vertex3D& v : mesh.Vertices) 
        {
            vmin.x = DirectX::XMMin(vmin.x, v.Pos.x);
            vmin.y = DirectX::XMMin(vmin.y, v.Pos.y);
            vmin.z = DirectX::XMMin(vmin.z, v.Pos.z);
            vmax.x = DirectX::XMMax(vmax.x, v.Pos.x);
            vmax.y = DirectX::XMMax(vmax.y, v.Pos.y);
            vmax.z = DirectX::XMMax(vmax.z, v.Pos.z);
        }
    }

    float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
    float dl = DirectX::XMMax(DirectX::XMMax(dx, dy), dz);
    float cx = (vmax.x + vmin.x) * 0.5f, cy = (vmax.y + vmin.y) * 0.5f,
        cz = (vmax.z + vmin.z) * 0.5f;

    for (MeshData& mesh : meshDatas)
    {
        for (Vertex3D& v : mesh.Vertices)
        {
            v.Pos.x = (v.Pos.x - cx) / dl;
            v.Pos.y = (v.Pos.y - cy) / dl;
            v.Pos.z = (v.Pos.z - cz) / dl;
        }
    }

    return meshDatas;
}
std::tuple<std::vector<MeshData>, AnimationData*> GeomatryGenerator::ReadFBXFile(const std::string& basePath, const std::string& filename, const std::string& animKey, const bool bIsRevertNormals)
{
    ModelLoader modelLoader;
    modelLoader.LoadWithAnimatnionData(basePath, filename, animKey, bIsRevertNormals); 
    GeomatryGenerator::GetInstance().normalize(Vector3(0.0f), 1.0f, modelLoader.MeshDatas, *modelLoader.pAnimData);
    return { modelLoader.MeshDatas, modelLoader.pAnimData };
}

void GeomatryGenerator::ParseAnimationClip(const std::string& basePath, const std::string& filename, const std::string& animKey, const std::string& clipKey)
{
    ModelLoader modelLoader;
    modelLoader.ParseAnimationClips(basePath, filename, animKey, clipKey);
}

void GeomatryGenerator::normalize(const Vector3 center, const float longestLength, std::vector<MeshData>& meshes, AnimationData& animData)
{
    // Normalize vertices
    Vector3 vmin(1000, 1000, 1000);
    Vector3 vmax(-1000, -1000, -1000);
    for (auto& mesh : meshes) 
    {
        for (auto& v : mesh.Vertices) 
        {
            vmin.x = XMMin(vmin.x, v.Pos.x);
            vmin.y = XMMin(vmin.y, v.Pos.y);
            vmin.z = XMMin(vmin.z, v.Pos.z);
            vmax.x = XMMax(vmax.x, v.Pos.x);
            vmax.y = XMMax(vmax.y, v.Pos.y);
            vmax.z = XMMax(vmax.z, v.Pos.z);
        }
    }

    float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
    float scale = longestLength / XMMax(XMMax(dx, dy), dz);
    //Vector3 translation = -(vmin + vmax) * 0.5f + center;
    Vector3 translation(0.0f, 0.0f, 0.0f);
    for (auto& mesh : meshes) 
    {
        auto& vertices = mesh.Vertices;
        auto& skinnedVertices = mesh.SkinnedVertices;

        for (uint32_t i = 0; i < skinnedVertices.size(); ++i)
        {
            vertices[i].Pos = (vertices[i].Pos) * scale;
            skinnedVertices[i].Pos = (skinnedVertices[i].Pos) * scale;
        }
    }

    // 애니메이션 데이터 보정에 사용
   animData.DefaultTransform = Matrix::CreateScale(scale);
}

}
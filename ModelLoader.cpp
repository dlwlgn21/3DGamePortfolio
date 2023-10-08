#include <filesystem>
#include <directxtk/SimpleMath.h>
#include <DirectXMesh.h>


#include "ModelLoader.h"
#include "Texture.h"
#include "GraphicsDatas.h"
#include "ResourcesManager.h"
#include "GraphicsKeyContainer.h"

using namespace DirectX::SimpleMath;
using namespace jh::graphics;

namespace jh
{

void ModelLoader::Load(const std::string& basePath, const std::string& filename) 
{
    this->BasePath = basePath;

    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(
        this->BasePath + filename,
        aiProcess_Triangulate | aiProcess_ConvertToLeftHanded
    );
    if (pScene == nullptr)
    {
        assert(false);
    }
    else
    {
        Matrix tr; // Initial transformation
        ProcessNode(pScene->mRootNode, pScene, tr);
    }
    UpdateTangents();
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, Matrix tr) 
{
    Matrix m;
    ai_real* temp = &node->mTransformation.a1;
    float* mTemp = &m._11;
    for (int t = 0; t < 16; t++) 
    {
        mTemp[t] = float(temp[t]);
    }
    m = m.Transpose() * tr;

    for (UINT i = 0; i < node->mNumMeshes; i++) 
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        auto newMesh = this->ProcessMesh(mesh, scene);
        for (auto& v : newMesh.Vertices) 
        {
            v.Pos = DirectX::SimpleMath::Vector3::Transform(v.Pos, m);
        }
        MeshDatas.push_back(newMesh);
    }

    for (UINT i = 0; i < node->mNumChildren; i++) 
    {
        this->ProcessNode(node->mChildren[i], scene, m);
    }
}

jh::graphics::MeshData ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    // Data to fill
    std::vector<Vertex3D> vertices;
    std::vector<UINT> indices;

    // Walk through each of the mesh's vertices
    for (UINT i = 0; i < mesh->mNumVertices; i++) 
    {
        Vertex3D vertex;

        vertex.Pos.x = mesh->mVertices[i].x;
        vertex.Pos.y = mesh->mVertices[i].y;
        vertex.Pos.z = mesh->mVertices[i].z;

        vertex.Normal.x = mesh->mNormals[i].x;
        vertex.Normal.y = mesh->mNormals[i].y;
        vertex.Normal.z = mesh->mNormals[i].z;
        vertex.Normal.Normalize();

        if (mesh->mTextureCoords[0]) 
        {
            vertex.UV.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.UV.y = (float)mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    for (UINT i = 0; i < mesh->mNumFaces; i++) 
    {
        aiFace face = mesh->mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    MeshData newMesh;
    newMesh.Vertices = vertices;
    newMesh.Indices = indices;

    // http://assimp.sourceforge.net/lib_html/materials.html
    assignTextureFileNames(mesh, scene, newMesh);
    return newMesh;
}


std::string ModelLoader::GetFullPathTextureFileNameOrEmptyString(aiMaterial* material, aiTextureType type)
{

    if (material->GetTextureCount(type) > 0) 
    {
        aiString filepath;
        material->GetTexture(type, 0, &filepath);
        std::string fullPath = this->BasePath + std::string(std::filesystem::path(filepath.C_Str()).filename().string());
        return fullPath;
    }
    else 
    {
        return "";
    }
}

void ModelLoader::UpdateTangents()
{
    using namespace std;
    using namespace DirectX;
    // https://github.com/microsoft/DirectXMesh/wiki/ComputeTangentFrame
    for (auto& mesh : this->MeshDatas)
    {
        vector<XMFLOAT3> positions(mesh.Vertices.size());
        vector<XMFLOAT3> normals(mesh.Vertices.size());
        vector<XMFLOAT2> texcoords(mesh.Vertices.size());
        vector<XMFLOAT3> tangents(mesh.Vertices.size());
        vector<XMFLOAT3> bitangents(mesh.Vertices.size());

        for (size_t i = 0; i < mesh.Vertices.size(); i++) {
            auto& v = mesh.Vertices[i];
            positions[i] = v.Pos;
            normals[i] = v.Normal;
            texcoords[i] = v.UV;
        }

        ComputeTangentFrame(
            mesh.Indices.data(), 
            mesh.Indices.size() / 3,
            positions.data(), 
            normals.data(), 
            texcoords.data(),
            mesh.Vertices.size(),
            tangents.data(),
            bitangents.data()
        );

        for (size_t i = 0; i < mesh.Vertices.size(); i++) 
        {
            mesh.Vertices[i].TangentModel = tangents[i];
        }
    }
}

void ModelLoader::assignTextureFileNames(aiMesh* mesh, const aiScene* scene, jh::graphics::MeshData& meshData)
{
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::string diffuseTextureFullPath = GetFullPathTextureFileNameOrEmptyString(material, aiTextureType_DIFFUSE);
        //assert(!diffuseTextureFullPath.empty());
        std::string normalTextureFullPath = GetFullPathTextureFileNameOrEmptyString(material, aiTextureType_NORMALS);
        //assert(!normalTextureFullPath.empty());

        meshData.DiffuseTextureFileFullPath = std::wstring(diffuseTextureFullPath.begin(), diffuseTextureFullPath.end());
        meshData.NormalTextureFileFullPath = std::wstring(normalTextureFullPath.begin(), normalTextureFullPath.end());
    }
}

}

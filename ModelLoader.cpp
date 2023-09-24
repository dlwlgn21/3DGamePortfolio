#include <filesystem>
#include <directxtk/SimpleMath.h>


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
    Matrix tr; // Initial transformation
    ProcessNode(pScene->mRootNode, pScene, tr);
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
        MeshData newMeshData = this->ProcessMesh(mesh, scene);

        for (auto& v : newMeshData.Vertices) {
            v.Pos = DirectX::SimpleMath::Vector3::Transform(v.Pos, m);
        }

        MeshDatas.push_back(newMeshData);
    }

    for (UINT i = 0; i < node->mNumChildren; i++) {
        this->ProcessNode(node->mChildren[i], scene, m);
    }
}
//
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
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) 
        {
            aiString filepath;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &filepath);
            std::string fullPath = this->BasePath + std::string(std::filesystem::path(filepath.C_Str()).filename().string());
            newMesh.TextureFileFullPath = fullPath;
        }
    }

    return newMesh;
}
}
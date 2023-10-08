#pragma once

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "MeshData.h"

namespace jh
{

class ModelLoader final
{
public:
    void Load(const std::string& basePath, const std::string& filename);
    void ProcessNode(aiNode* node, const aiScene* scene, DirectX::SimpleMath::Matrix tr);
    jh::graphics::MeshData ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::string GetFullPathTextureFileNameOrEmptyString(aiMaterial* material, aiTextureType type);
    void UpdateTangents();

private:
    void assignTextureFileNames(aiMesh* mesh, const aiScene* scene, jh::graphics::MeshData& meshData);

public:
    std::string BasePath;
    std::vector<jh::graphics::MeshData> MeshDatas;
};
}


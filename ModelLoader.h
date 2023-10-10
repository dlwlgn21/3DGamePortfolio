#pragma once

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "MeshData.h"
#include "AnimationData.h"


namespace jh
{

class ModelLoader final
{
public:
    void Load(const std::string& basePath, const std::string& filename);
    void LoadWithAnimatnionData(const std::string& basePath, const std::string& filename, const bool bIsRevertNormals);

private:
    void processNode(aiNode* node, const aiScene* scene, DirectX::SimpleMath::Matrix tr);
    void processNodeRecursive(aiNode* pNode, const aiScene* pScene, DirectX::SimpleMath::Matrix tr);
    const aiNode* findParentRecursive(const aiNode* pNode);

    jh::graphics::MeshData processMesh(aiMesh* mesh, const aiScene* scene);
    std::string getFullPathTextureFileNameOrEmptyString(aiMaterial* material, aiTextureType type);
    void updateTangents();

    void findDeformingBonesForAssigningBoneNameKeyMapping(const aiScene* pScene);
    void updateBoneIndiceSequenceRecursiveForBoneNameIndexValueMapping(aiNode* pNode, int* counter);
    void readAnimationClips(const aiScene* pScene);
    void assignTextureFileNames(aiMesh* mesh, const aiScene* scene, jh::graphics::MeshData& meshData);

public:
    std::string                         BasePath;
    std::vector<jh::graphics::MeshData> MeshDatas;

    jh::graphics::AnimationData         AnimData;
};
}


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
    void LoadWithAnimatnionData(const std::string& basePath, const std::string& filename, const std::string& animKey, const bool bIsRevertNormals);

private:
    void processNode(aiNode* node, const aiScene* scene, DirectX::SimpleMath::Matrix tr);
    void processNodeRecursiveForParentBoneIndexing(aiNode* pNode, const aiScene* pScene, DirectX::SimpleMath::Matrix tr);
    const aiNode* findParentRecursive(const aiNode* pNode);

    jh::graphics::MeshData parseVerticesAndIndicesAndTexture(aiMesh* pMesh, const aiScene* pScene);
    std::string getFullPathTextureFileNameOrEmptyString(aiMaterial* pMaterial, const aiScene* pScene, aiTextureType type);
    void updateTangents();

    void findDeformingBonesForAssigningBoneNameKeyMapping(const aiScene* pScene);
    void updateBoneIndiceSequenceRecursiveForBoneNameIndexValueMapping(aiNode* pNode, int* counter);
    void parseAnimationClips(const aiScene* pScene);
    void assignTextureFileNames(aiMesh* mesh, const aiScene* scene, jh::graphics::MeshData& meshData);
    
    void printNameIndexMap();
    void printParentIndices();
public:
    std::string                         BasePath;
    std::vector<jh::graphics::MeshData> MeshDatas;

    jh::graphics::AnimationData*        pAnimData = nullptr;
};
}


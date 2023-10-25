#define _CRT_SECURE_NO_WARNINGS
#include <filesystem>
#include <iostream>
#include <fstream>
#include <directxtk/SimpleMath.h>
#include <DirectXMesh.h>
#include <algorithm>
#include <filesystem>
#include <fstream>

#include "ModelLoader.h"
#include "Texture.h"
#include "GraphicsDatas.h"
#include "ResourcesManager.h"
#include "GraphicsKeyContainer.h"
#include "AnimationDataManager.h"

using namespace DirectX::SimpleMath;
using namespace jh::graphics;

namespace jh
{

std::string GetExtension(const std::string& filename)
{
    std::string ext(std::filesystem::path(filename).extension().string());
    transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}


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
        processNode(pScene->mRootNode, pScene, tr);
    }
    updateTangents();
}

void ModelLoader::LoadWithAnimatnionData(const std::string& basePath, const std::string& filename, const std::string& animKey, const bool bIsRevertNormals)
{
    if (GetExtension(filename) == ".gltf")
    {
        assert(false);
    }

    BasePath = basePath; // 텍스춰 읽어들일 때 필요

    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(
        BasePath + filename,
        aiProcess_Triangulate | aiProcess_ConvertToLeftHanded
    );
    assert(pScene != nullptr);
    auto& AnimDataManager = AnimationDataManager::GetInstance();
    pAnimData = nullptr;
    if (AnimDataManager.GetAnimDataOrNull(animKey) == nullptr)
    {
        AnimDataManager.InsertAnimationData(animKey, std::make_unique<jh::graphics::AnimationData>());
        pAnimData = AnimDataManager.GetAnimDataOrNull(animKey);
    }
    else
    {
        pAnimData = AnimDataManager.GetAnimDataOrNull(animKey);
    }

    int counter = 0;
    assert(pAnimData != nullptr);
    findDeformingBonesForAssigningBoneNameKeyMapping(pScene);
    updateBoneIndiceSequenceRecursiveForBoneNameIndexValueMapping(pScene->mRootNode, &counter);
    pAnimData->BoneIndexToNameArray.resize(pAnimData->BoneNameIndexMap.size());
    for (auto& i : pAnimData->BoneNameIndexMap)
    {
        pAnimData->BoneIndexToNameArray[i.second] = i.first;
    }
    pAnimData->BoneParentIndexArray.resize(pAnimData->BoneNameIndexMap.size(), -1);
    pAnimData->BoneTransformMatrixArray.resize(pAnimData->BoneNameIndexMap.size());
    pAnimData->OffsetMatrixArray.resize(pAnimData->BoneNameIndexMap.size());
    Matrix tr = Matrix::Identity;
    // 각 Bone들의 Parent들을 Traverse 하면서 Assign함.
    processNodeRecursiveForParentBoneIndexing(pScene->mRootNode, pScene, tr);
    updateTangents();
}

void ModelLoader::ParseAnimationClips(const std::string& basePath, const std::string& filename, const std::string& animKey, const std::string& clipKey)
{
    if (GetExtension(filename) == ".gltf")
    {
        assert(false);
    }

    BasePath = basePath; // 텍스춰 읽어들일 때 필요

    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(
        BasePath + filename,
        aiProcess_Triangulate | aiProcess_ConvertToLeftHanded
    );
    assert(pScene != nullptr);
    auto& AnimDataManager = AnimationDataManager::GetInstance();
    pAnimData = nullptr;
    if (AnimDataManager.GetAnimDataOrNull(animKey) == nullptr)
    {
        assert(false);
    }
    else
    {
        pAnimData = AnimDataManager.GetAnimDataOrNull(animKey);
    }
    assert(pScene->HasAnimations());
    auto iter = pAnimData->ClipMap.insert(std::make_pair(clipKey, AnimationClip()));
    if (iter.second == false)
    {
        assert(false);
    }
    for (uint32_t i = 0; i < pScene->mNumAnimations; i++)
    {
        AnimationClip& clip = iter.first->second;
        const aiAnimation* pAnimation = pScene->mAnimations[i];
        clip.Duration = pAnimation->mDuration;
        clip.TicksPerSec = pAnimation->mTicksPerSecond;
        clip.KeyBoneAndFrame2DArrays.resize(pAnimData->BoneNameIndexMap.size());
        clip.NumChannelsActuallyNumsBones = pAnimation->mNumChannels;
        clip.TotalAnimTime = static_cast<float>(clip.Duration / clip.TicksPerSec);
        double tickPerSecond = 1 / clip.TicksPerSec;
        /*
        * 왜 채널이란 이름이 사용되냐고? 각 본들의 움직임이 각각의 채널로 제공이 되기 떄문임.
        * 각 본들의 움직임이 시간에 따른 변화 곡선임.
        * 하나의 변화 곡선을 하나의 채널에다가 데이터를 쏴준다.
        */
        for (uint32_t c = 0; c < pAnimation->mNumChannels; c++)
        {
            const aiNodeAnim* pNodeAnim = pAnimation->mChannels[c];
            const int32_t boneIndex = pAnimData->BoneNameIndexMap[pNodeAnim->mNodeName.C_Str()];
            clip.KeyBoneAndFrame2DArrays[boneIndex].resize(pNodeAnim->mNumPositionKeys);
            // 요기서 시간에 따라 본들이 어떻게 변하는지, Assign 함.
            for (uint32_t frame = 0; frame < pNodeAnim->mNumPositionKeys; frame++)
            {
                const auto pos = pNodeAnim->mPositionKeys[frame].mValue;
                const auto rot = pNodeAnim->mRotationKeys[frame].mValue;
                const auto scale = pNodeAnim->mScalingKeys[frame].mValue;
                /* TODO : Added Part AT 10-25-18:00 */

                AnimationClip::Key& key = clip.KeyBoneAndFrame2DArrays[boneIndex][frame];
                key.Pos =       { pos.x, pos.y, pos.z };
                key.Rot =       Quaternion(rot.x, rot.y, rot.z, rot.w);
                key.Scale =     { scale.x, scale.y, scale.z };
                key.PositionTime    = pNodeAnim->mPositionKeys[frame].mTime * tickPerSecond;
                key.ScaleTime       = pNodeAnim->mRotationKeys[frame].mTime * tickPerSecond;
                key.RotationTime    = pNodeAnim->mRotationKeys[frame].mTime * tickPerSecond;
            }
        }
    }

}

void ModelLoader::processNode(aiNode* node, const aiScene* scene, Matrix tr) 
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
        auto newMesh = this->parseVerticesAndIndicesAndTexture(mesh, scene);
        for (auto& v : newMesh.Vertices) 
        {
            v.Pos = DirectX::SimpleMath::Vector3::Transform(v.Pos, m);
        }
        MeshDatas.push_back(newMesh);
    }

    for (UINT i = 0; i < node->mNumChildren; i++) 
    {
        this->processNode(node->mChildren[i], scene, m);
    }
}

void ModelLoader::processNodeRecursiveForParentBoneIndexing(aiNode* pNode, const aiScene* pScene, DirectX::SimpleMath::Matrix mat)
{
    // 사용되는 Parent의 Bone을 찾아서 Parent의 Index를 저장함.
    if (pNode->mParent != nullptr &&
        pAnimData->BoneNameIndexMap.count(pNode->mName.C_Str()) == 1 &&
        findParentRecursive(pNode->mParent))
    {
        const int32_t boneIndex = pAnimData->BoneNameIndexMap[pNode->mName.C_Str()];
        pAnimData->BoneParentIndexArray[boneIndex] = pAnimData->BoneNameIndexMap[findParentRecursive(pNode->mParent)->mName.C_Str()];
    }

    Matrix m(&pNode->mTransformation.a1);
    m = m.Transpose() * mat;

    for (UINT i = 0; i < pNode->mNumMeshes; i++)
    {
        aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[i]];
        MeshData meshData = this->parseVerticesAndIndicesAndTexture(pMesh, pScene);
        // TODO : 여기 바꿈. 나중에 문제 없는지 체크해야함. 그 전에는 meshData.Vertices에 대해서 반복문 돌았었엉.
        for (auto& v : meshData.SkinnedVertices)
        {
            v.Pos = DirectX::SimpleMath::Vector3::Transform(v.Pos, m);
        }        
        MeshDatas.push_back(meshData);
    }
    for (UINT i = 0; i < pNode->mNumChildren; i++)
    {
        this->processNodeRecursiveForParentBoneIndexing(pNode->mChildren[i], pScene, m);
    }
}

jh::graphics::MeshData ModelLoader::parseVerticesAndIndicesAndTexture(aiMesh* pMesh, const aiScene* pScene)
{
    MeshData newMesh;
    auto& vertices = newMesh.Vertices;
    auto& indices = newMesh.Indices;
    vertices.reserve(pMesh->mNumVertices);
    for (UINT i = 0; i < pMesh->mNumVertices; i++)
    {
        Vertex3D vertex;

        vertex.Pos.x = pMesh->mVertices[i].x;
        vertex.Pos.y = pMesh->mVertices[i].y;
        vertex.Pos.z = pMesh->mVertices[i].z;

        vertex.Normal.x = pMesh->mNormals[i].x;
        vertex.Normal.y = pMesh->mNormals[i].y;
        vertex.Normal.z = pMesh->mNormals[i].z;
        vertex.Normal.Normalize();

        if (pMesh->mTextureCoords[0])
        {
            vertex.UV.x = (float)pMesh->mTextureCoords[0][i].x;
            vertex.UV.y = (float)pMesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    for (UINT i = 0; i < pMesh->mNumFaces; i++)
    {
        aiFace face = pMesh->mFaces[i];
        indices.reserve(face.mNumIndices);
        for (UINT j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    assignTextureFileNames(pMesh, pScene, newMesh);
    if (pAnimData == nullptr || pAnimData->BoneNameIndexMap.size() == 0)
    {
        return newMesh;
    }

    if (pMesh->HasBones())
    {
        auto& skinnedVertices = newMesh.SkinnedVertices;
        std::vector<std::vector<float>> boneWeights(vertices.size());
        std::vector<std::vector<uint8_t>> boneIndices(vertices.size());

        int count = 0;
        for (uint32_t i = 0; i < pAnimData->BoneNameIndexMap.size(); ++i)
        {
            const aiBone* pBone = pMesh->mBones[i];
            const uint32_t boneId = pAnimData->BoneNameIndexMap[pBone->mName.C_Str()];

            pAnimData->OffsetMatrixArray[boneId] = Matrix((float*)&pBone->mOffsetMatrix).Transpose();

            // 이 뼈가 영향을 주는 Vertex의 개수
            for (uint32_t j = 0; j < pBone->mNumWeights; j++)
            {
                aiVertexWeight& weight = pBone->mWeights[j];
                assert(weight.mVertexId < boneIndices.size());
                boneIndices[weight.mVertexId].push_back(boneId);
                boneWeights[weight.mVertexId].push_back(weight.mWeight);
            }
        }


        int maxBones = 0;
        for (int i = 0; i < boneWeights.size(); i++)
        {
            maxBones = std::max(maxBones, static_cast<int>(boneWeights[i].size()));
        }

        float maxNum = -10000.0f;

        skinnedVertices.reserve(vertices.size());
        skinnedVertices.resize(vertices.size());
        for (int i = 0; i < vertices.size(); ++i)
        {
            skinnedVertices[i].Pos = vertices[i].Pos;
            skinnedVertices[i].Normal = vertices[i].Normal;
            skinnedVertices[i].UV = vertices[i].UV;

            for (int j = 0; j < boneWeights[i].size(); ++j)
            {
                skinnedVertices[i].BlendWeights[j] = boneWeights[i][j];
                skinnedVertices[i].BoneIndicies[j] = boneIndices[i][j];
            }
        }
    }
    return newMesh;
}


std::string ModelLoader::getFullPathTextureFileNameOrEmptyString(aiMaterial* pMaterial, const aiScene* pScene, aiTextureType type)
{
    if (pMaterial->GetTextureCount(type) > 0)
    {
        aiString filepath;
        pMaterial->GetTexture(type, 0, &filepath);
        std::string fullPath = BasePath + std::string(std::filesystem::path(filepath.C_Str()).filename().string());

        // 1. 실제로 파일이 존재하는지 확인
        if (!std::filesystem::exists(fullPath))
        {
            // 2. 파일이 없을 경우 혹시 fbx 자체에 Embedded인지 확인
            const aiTexture* pTexture = pScene->GetEmbeddedTexture(filepath.C_Str());
            if (pTexture != nullptr)
            {
                // 3. Embedded texture가 존재하고 png일 경우 저장
                if (std::string(pTexture->achFormatHint).find("png") != std::string::npos)
                {
                    std::ofstream fs(fullPath.c_str(), std::ios::binary | std::ios::out);
                    fs.write((char*)pTexture->pcData, pTexture->mWidth);
                    fs.close();
                    // 참고: compressed format일 경우 texture->mHeight가 0
                }
            }
            else
            {
                assert(false);
            }
        }
        else
        {
            return fullPath;
        }

        return fullPath;

    }
    else
    {
        return "";
    }
}

void ModelLoader::updateTangents()
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

        if (mesh.SkinnedVertices.size() != 0)
        {
            for (size_t i = 0; i < mesh.Vertices.size(); i++)
            {
                mesh.Vertices[i].TangentModel = tangents[i];
                mesh.SkinnedVertices[i].TangentModel = tangents[i];
            }
        }
        else
        {
            for (size_t i = 0; i < mesh.Vertices.size(); i++) 
            {
                mesh.Vertices[i].TangentModel = tangents[i];
            }
        }
    }
}

void ModelLoader::findDeformingBonesForAssigningBoneNameKeyMapping(const aiScene* pScene)
{
    for (uint32_t i = 0; i < pScene->mNumMeshes; i++)
    {
        const aiMesh* pMesh = pScene->mMeshes[i];
        if (pMesh->HasBones())
        {
            for (uint32_t i = 0; i < pMesh->mNumBones; i++)
            {
                const aiBone* pBone = pMesh->mBones[i];
                pAnimData->BoneNameIndexMap[pBone->mName.C_Str()] = -1;
            }
        }
    }
}

void ModelLoader::updateBoneIndiceSequenceRecursiveForBoneNameIndexValueMapping(aiNode* pNode, int* counter)
{
    //static int id = 0;
    if (pNode != nullptr)
    {
        if (pAnimData->BoneNameIndexMap.count(pNode->mName.C_Str()) == 1)
        {
            pAnimData->BoneNameIndexMap[pNode->mName.C_Str()] = *counter;
            *counter += 1;
        }
        for (UINT i = 0; i < pNode->mNumChildren; i++)
        {
            updateBoneIndiceSequenceRecursiveForBoneNameIndexValueMapping(pNode->mChildren[i], counter);
        }
    }
}

void ModelLoader::assignTextureFileNames(aiMesh* pMesh, const aiScene* pScene, jh::graphics::MeshData& meshData)
{
    if (pMesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = pScene->mMaterials[pMesh->mMaterialIndex];
        std::string diffuseTextureFullPath = getFullPathTextureFileNameOrEmptyString(material, pScene, aiTextureType_DIFFUSE);
        //assert(!diffuseTextureFullPath.empty());
        std::string normalTextureFullPath = getFullPathTextureFileNameOrEmptyString(material, pScene, aiTextureType_NORMALS);
        //assert(!normalTextureFullPath.empty());

        meshData.DiffuseTextureFileFullPath = std::wstring(diffuseTextureFullPath.begin(), diffuseTextureFullPath.end());
        meshData.NormalTextureFileFullPath = std::wstring(normalTextureFullPath.begin(), normalTextureFullPath.end());
    }
}
void ModelLoader::printNameIndexMap()
{
    std::cout << "-----------\nDebug AnimationDataInfo BoneNameIndexMap Output START\n-----------" << std::endl;
    std::cout << "Nums Of BoneNameIndexMap : " << pAnimData->BoneIndexToNameArray.size() << std::endl;
    for (auto& i : pAnimData->BoneNameIndexMap)
    {
        std::cout << "NameIndex pair : " << i.first << " " << i.second << std::endl;
    }
    std::cout << "Num BoneIndexToName : " << pAnimData->BoneIndexToNameArray.size() << std::endl;
    for (size_t i = 0; i < pAnimData->BoneIndexToNameArray.size(); i++)
    {
        std::cout << "BoneIndex: " << i << " " << pAnimData->BoneIndexToNameArray[i] << std::endl;
    }
    std::cout << "-----------\nDebug AnimationDataInfo BoneNameIndexMap Output END\n-----------" << std::endl;
}
void ModelLoader::printParentIndices()
{
    std::cout << "-----------\nDebug AnimationDataInfo Parent Output START\n-----------" << std::endl;
    std::cout << "Num boneIndexToName : " << pAnimData->BoneIndexToNameArray.size() << std::endl;
    for (size_t i = 0; i < pAnimData->BoneIndexToNameArray.size(); i++)
    {
        std::cout << "BoneIndex: "
            << i
            << " "
            << pAnimData->BoneIndexToNameArray[i]
            << " , Parent: "
            << (pAnimData->BoneParentIndexArray[i] == -1 ? "NONE" : pAnimData->BoneIndexToNameArray[pAnimData->BoneParentIndexArray[i]])
            << std::endl;
    }
    std::cout << "-----------\nDebug AnimationDataInfo Parent Output END\n-----------" << std::endl;
}
const aiNode* ModelLoader::findParentRecursive(const aiNode* pNode)
{
    if (!pNode)
    {
        return nullptr;
    }
    if (pAnimData->BoneNameIndexMap.count(pNode->mName.C_Str()) > 0)
    {
        return pNode;
    }
    return findParentRecursive(pNode->mParent);
}

void ModelLoader::parseAnimationClips(const aiScene* pScene)
{

}

}

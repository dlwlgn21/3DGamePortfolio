#include <filesystem>
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


/*
 * AnimationData 읽는 용도.
 * 여러개의 Bone들이 있고 Tree 구조임
 * 그 중에서 Vertex에 영향을 주는 Bone들은 일부임
 * Vertex에 영향을 주는 Bone들과 Prarent들까지 포함해서 읽어와서 저장.
 */
void ModelLoader::LoadWithAnimatnionData(const std::string& basePath, const std::string& filename, const bool bIsRevertNormals)
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
    // ReadFile()에서 경우에 따라서 여러가지 옵션들 설정 가능
    // aiProcess_JoinIdenticalVertices | aiProcess_PopulateArmatureData |
    // aiProcess_SplitByBoneCount |
    // aiProcess_Debone); // aiProcess_LimitBoneWeights

    if (pScene != nullptr)
    {
        /*
         * 1. 모든 메쉬에 대해서 버텍스에 영향을 주는 뼈들의 목록을 만든다.
         *    BoneNameIndexMap의 Key들을 결정함.
         */
        findDeformingBonesForAssigningBoneNameKeyMapping(pScene);

        int counter = 0;
        /*
         * 2. 트리 구조를 따라 업데이트 순서대로 뼈들의 인덱스를 결정한다.
         *    부모의 인덱스가 자식의 인덱스보다 항상 작을 수 있도록 인덱싱함.
         *    BoneNameIndexMap 완성.
         */
        updateBoneIndiceSequenceRecursiveForBoneNameIndexValueMapping(pScene->mRootNode, &counter);

        // 3. 완성된 BoneNameIndexMap을 이용해서 업데이트 순서대로 뼈 이름 저장 (boneIndexToName) 즉, 부모에서 자식 순서로 저장함.
        AnimData.BoneIndexToNameArray.resize(AnimData.BoneNameIndexMap.size());
        for (auto& i : AnimData.BoneNameIndexMap)
        {
            AnimData.BoneIndexToNameArray[i.second] = i.first;
        }

#pragma region DEBUGGING_NAME_INDEX_MAP_OUPUT
        // 디버깅용
        std::cout << "-----------\nDebug AnimationDataInfo BoneNameIndexMap Output START\n-----------" << std::endl;
        std::cout << "Nums Of BoneNameIndexMap : " << AnimData.BoneIndexToNameArray.size() << std::endl;
        for (auto& i : AnimData.BoneNameIndexMap)
        {
            std::cout << "NameIndex pair : " << i.first << " " << i.second << std::endl;
        }
        std::cout << "Num BoneIndexToName : " << AnimData.BoneIndexToNameArray.size() << std::endl;
        for (size_t i = 0; i < AnimData.BoneIndexToNameArray.size(); i++)
        {
            std::cout << "BoneIndex: " << i << " " << AnimData.BoneIndexToNameArray[i] << std::endl;
        }
        std::cout << "-----------\nDebug AnimationDataInfo BoneNameIndexMap Output END\n-----------" << std::endl;
#pragma endregion

        // 각 Bone의 Parent Index를 저장할 준비
        AnimData.BoneParentIndexArray.resize(AnimData.BoneNameIndexMap.size(), -1);
        Matrix tr; 
        // 각 Bone들의 Parent들을 Traverse 하면서 Assign함.
        processNodeRecursive(pScene->mRootNode, pScene, tr);

#pragma region DEBUGGING_2_PARENT_OUPUT
        // 디버깅용
        std::cout << "-----------\nDebug AnimationDataInfo Parent Output START\n-----------" << std::endl;
        std::cout << "Num boneIndexToName : " << AnimData.BoneIndexToNameArray.size() << std::endl; 
        for (size_t i = 0; i < AnimData.BoneIndexToNameArray.size(); i++)
        {
            std::cout << "BoneIndex: " 
                 << i 
                 << " " 
                 << AnimData.BoneIndexToNameArray[i] 
                 << " , Parent: "
                 << (AnimData.BoneParentIndexArray[i] == -1 ? "NONE" : AnimData.BoneIndexToNameArray[AnimData.BoneParentIndexArray[i]])
                 << std::endl;
        }
        std::cout << "-----------\nDebug AnimationDataInfo Parent Output END\n-----------" << std::endl;
#pragma endregion


        // 여기까지 오면 AnimData의 BoneNameIndexMapping이 끝나고, Indexing 관련 작업들이 완료됨.
        // 애니메이션 클립 정보 읽기
        // 이곳이 정말 애니메이션 정보 읽는 부분
        if (pScene->HasAnimations())
        {
            readAnimationClips(pScene);
        }

        // UpdateNormals(this->meshes); // Vertex Normal을 직접 계산 (참고용)
        updateTangents();
    }
    else
    {
        assert(false);
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
        auto newMesh = this->processMesh(mesh, scene);
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

void ModelLoader::processNodeRecursive(aiNode* pNode, const aiScene* pScene, DirectX::SimpleMath::Matrix tr)
{
    if (pNode->mParent &&
        AnimData.BoneNameIndexMap.count(pNode->mName.C_Str()) &&
        findParentRecursive(pNode->mParent))
    {
        const auto boneId = AnimData.BoneNameIndexMap[pNode->mName.C_Str()];
        AnimData.BoneParentIndexArray[boneId] = AnimData.BoneNameIndexMap[findParentRecursive(pNode->mParent)->mName.C_Str()];
    }

    Matrix m(&pNode->mTransformation.a1);
    m = m.Transpose() * tr;

    for (UINT i = 0; i < pNode->mNumMeshes; i++)
    {
        aiMesh* mesh = pScene->mMeshes[pNode->mMeshes[i]];
        auto newMesh = this->processMesh(mesh, pScene);
        for (auto& v : newMesh.Vertices)
        {
            v.Pos = DirectX::SimpleMath::Vector3::Transform(v.Pos, m);
        }
        MeshDatas.push_back(newMesh);
    }

    for (UINT i = 0; i < pNode->mNumChildren; i++)
    {
        this->processNodeRecursive(pNode->mChildren[i], pScene, m);
    }
}

jh::graphics::MeshData ModelLoader::processMesh(aiMesh* pMesh, const aiScene* pScene) 
{
    // Data to fill
    MeshData newMesh;
    auto& vertices = newMesh.Vertices;
    auto& indices = newMesh.Indices;
    vertices.reserve(pMesh->mNumVertices);

    // Walk through each of the mesh's vertices
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
        for (UINT j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    if (pMesh->HasBones())
    {
        if (AnimData.BoneNameIndexMap.size() == 0)
        {
            assignTextureFileNames(pMesh, pScene, newMesh);
            return newMesh;
        }
        auto& skinnedVertices = newMesh.SkinnedVertices;
        std::vector<std::vector<float>> boneWeights(vertices.size());
        std::vector<std::vector<uint8_t>> boneIndices(vertices.size());
        AnimData.OffsetMatrixArray.resize(AnimData.BoneNameIndexMap.size());
        AnimData.BoneTransformMatrixArray.resize(AnimData.BoneNameIndexMap.size());



        int count = 0;
        for (uint32_t i = 0; i < pMesh->mNumBones; ++i)
        {
            const aiBone* bone = pMesh->mBones[i];

            // 디버깅
            // cout << "BoneMap " << count++ << " : " << bone->mName.C_Str()
            //     << " NumBoneWeights = " << bone->mNumWeights << endl;

            const uint32_t boneId = AnimData.BoneNameIndexMap[bone->mName.C_Str()];

            AnimData.OffsetMatrixArray[boneId] = Matrix((float*)&bone->mOffsetMatrix).Transpose();

            // 이 뼈가 영향을 주는 Vertex의 개수
            for (uint32_t j = 0; j < bone->mNumWeights; j++)
            {
                aiVertexWeight weight = bone->mWeights[j];
                assert(weight.mVertexId < boneIndices.size());
                boneIndices[weight.mVertexId].push_back(boneId);
                boneWeights[weight.mVertexId].push_back(weight.mWeight);
            }
        }

        // 예전에는 Vertex 하나에 영향을 주는 Bone은 최대 4개
        // 요즘은 더 많을 수도 있는데 모델링 소프트웨어에서 조정하거나
        // 읽어들이면서 weight가 너무 작은 것들은 뺄 수도 있음

        int maxBones = 0;
        for (int i = 0; i < boneWeights.size(); i++)
        {
            maxBones = std::max(maxBones, int(boneWeights[i].size()));
        }

        std::cout << "Max number of influencing bones per vertex = " << maxBones << std::endl;
        skinnedVertices.reserve(vertices.size());
        skinnedVertices.resize(vertices.size());

        for (int i = 0; i < vertices.size(); i++)
        {
            skinnedVertices[i].Pos = vertices[i].Pos;
            skinnedVertices[i].Normal = vertices[i].Normal;
            skinnedVertices[i].UV = vertices[i].UV;

            for (int j = 0; j < boneWeights[i].size(); j++)
            {
                skinnedVertices[i].BlendWeights[j] = boneWeights[i][j];
                skinnedVertices[i].BoneIndicies[j] = boneIndices[i][j];
            }
        }
    }
    
    // http://assimp.sourceforge.net/lib_html/materials.html
    assignTextureFileNames(pMesh, pScene, newMesh);
    return newMesh;
}


std::string ModelLoader::getFullPathTextureFileNameOrEmptyString(aiMaterial* material, aiTextureType type)
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


// 버텍스의 변형에 직접적으로 참여하는 Bone들의 목록을 만듦
void ModelLoader::findDeformingBonesForAssigningBoneNameKeyMapping(const aiScene* pScene)
{
    for (uint32_t i = 0; i < pScene->mNumMeshes; i++)
    {
        const auto* pMesh = pScene->mMeshes[i];
        if (pMesh->HasBones())
        {
            for (uint32_t i = 0; i < pMesh->mNumBones; i++)
            {
                const aiBone* pBone = pMesh->mBones[i];

                // bone과 대응되는 node의 이름은 동일
                // 뒤에서 node 이름으로 부모를 찾을 수 있음
                // 실제로 렌더링 할 때에는 BoneName을 사용하진 않음. 하지만 파일을 읽어들일 떄에는 사용함.
                AnimData.BoneNameIndexMap[pBone->mName.C_Str()] = -1;
                // 주의: 뼈의 순서가 업데이트 순서는 아님
                // 기타: bone->mWeights == 0일 경우에도 포함시켰음
                // 기타: bone->mNode = 0이라서 사용 불가
            }
        }
    }
}

void ModelLoader::updateBoneIndiceSequenceRecursiveForBoneNameIndexValueMapping(aiNode* pNode, int* counter)
{
    static int id = 0;
    if (pNode)
    {
        if (AnimData.BoneNameIndexMap.count(pNode->mName.C_Str()))
        {
            AnimData.BoneNameIndexMap[pNode->mName.C_Str()] = *counter;
            *counter += 1;
        }
        for (UINT i = 0; i < pNode->mNumChildren; i++)
        {
            updateBoneIndiceSequenceRecursiveForBoneNameIndexValueMapping(pNode->mChildren[i], counter);
        }
    }
}

void ModelLoader::assignTextureFileNames(aiMesh* mesh, const aiScene* scene, jh::graphics::MeshData& meshData)
{
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::string diffuseTextureFullPath = getFullPathTextureFileNameOrEmptyString(material, aiTextureType_DIFFUSE);
        //assert(!diffuseTextureFullPath.empty());
        std::string normalTextureFullPath = getFullPathTextureFileNameOrEmptyString(material, aiTextureType_NORMALS);
        //assert(!normalTextureFullPath.empty());

        meshData.DiffuseTextureFileFullPath = std::wstring(diffuseTextureFullPath.begin(), diffuseTextureFullPath.end());
        meshData.NormalTextureFileFullPath = std::wstring(normalTextureFullPath.begin(), normalTextureFullPath.end());
    }
}
const aiNode* ModelLoader::findParentRecursive(const aiNode* pNode)
{
    if (!pNode)
    {
        return nullptr;
    }
    if (AnimData.BoneNameIndexMap.count(pNode->mName.C_Str()) > 0)
    {
        return pNode;
    }
    return findParentRecursive(pNode->mParent);
}

void ModelLoader::readAnimationClips(const aiScene* pScene)
{
    AnimData.ClipArray.resize(pScene->mNumAnimations);

    for (uint32_t i = 0; i < pScene->mNumAnimations; i++)
    {
        AnimationClip& clip = AnimData.ClipArray[i];

        const aiAnimation* pAnimation = pScene->mAnimations[i];

        clip.Duration = pAnimation->mDuration;
        clip.TicksPerSec = pAnimation->mTicksPerSecond;
        clip.KeyBoneAndFrame2DArrays.resize(AnimData.BoneNameIndexMap.size());
        clip.NumChannelsActuallyNumsBones = pAnimation->mNumChannels;

        /*
        * 왜 채널이란 이름이 사용되냐고? 각 본들의 움직임이 각각의 채널로 제공이 되기 떄문임.
        * 각 본들의 움직임이 시간에 따른 변화 곡선임.
        * 하나의 변화 곡선을 하나의 채널에다가 데이터를 쏴준다.
        */
        for (uint32_t c = 0; c < pAnimation->mNumChannels; c++)
        {
            const aiNodeAnim* nodeAnim = pAnimation->mChannels[c];
            const int boneIndex = AnimData.BoneNameIndexMap[nodeAnim->mNodeName.C_Str()];
            clip.KeyBoneAndFrame2DArrays[boneIndex].resize(nodeAnim->mNumPositionKeys);

            // 요기서 시간에 따라 본들이 어떻게 변하는지, Assign 함.
            for (uint32_t frame = 0; frame < nodeAnim->mNumPositionKeys; frame++)
            {
                const auto pos = nodeAnim->mPositionKeys[frame].mValue;
                const auto rot = nodeAnim->mRotationKeys[frame].mValue;
                const auto scale = nodeAnim->mScalingKeys[frame].mValue;
                AnimationClip::Key& key = clip.KeyBoneAndFrame2DArrays[boneIndex][frame];
                key.Pos = { pos.x, pos.y, pos.z };
                key.Rot = Quaternion(rot.x, rot.y, rot.z, rot.w);
                key.Scale = { scale.x, scale.y, scale.z };
            }
        }
    }
}

}

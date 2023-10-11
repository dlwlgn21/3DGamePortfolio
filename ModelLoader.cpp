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
 * AnimationData �д� �뵵.
 * �������� Bone���� �ְ� Tree ������
 * �� �߿��� Vertex�� ������ �ִ� Bone���� �Ϻ���
 * Vertex�� ������ �ִ� Bone��� Prarent����� �����ؼ� �о�ͼ� ����.
 */
void ModelLoader::LoadWithAnimatnionData(const std::string& basePath, const std::string& filename, const bool bIsRevertNormals)
{
    if (GetExtension(filename) == ".gltf")
    {
        assert(false);
    }

    BasePath = basePath; // �ؽ��� �о���� �� �ʿ�

    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(
        BasePath + filename,
        aiProcess_Triangulate | aiProcess_ConvertToLeftHanded
    );
    // ReadFile()���� ��쿡 ���� �������� �ɼǵ� ���� ����
    // aiProcess_JoinIdenticalVertices | aiProcess_PopulateArmatureData |
    // aiProcess_SplitByBoneCount |
    // aiProcess_Debone); // aiProcess_LimitBoneWeights

    if (pScene != nullptr)
    {
        /*
         * 1. ��� �޽��� ���ؼ� ���ؽ��� ������ �ִ� ������ ����� �����.
         *    BoneNameIndexMap�� Key���� ������.
         */
        findDeformingBonesForAssigningBoneNameKeyMapping(pScene);

        int counter = 0;
        /*
         * 2. Ʈ�� ������ ���� ������Ʈ ������� ������ �ε����� �����Ѵ�.
         *    �θ��� �ε����� �ڽ��� �ε������� �׻� ���� �� �ֵ��� �ε�����.
         *    BoneNameIndexMap �ϼ�.
         */
        updateBoneIndiceSequenceRecursiveForBoneNameIndexValueMapping(pScene->mRootNode, &counter);

        // 3. �ϼ��� BoneNameIndexMap�� �̿��ؼ� ������Ʈ ������� �� �̸� ���� (boneIndexToName) ��, �θ𿡼� �ڽ� ������ ������.
        AnimData.BoneIndexToNameArray.resize(AnimData.BoneNameIndexMap.size());
        for (auto& i : AnimData.BoneNameIndexMap)
        {
            AnimData.BoneIndexToNameArray[i.second] = i.first;
        }

#pragma region DEBUGGING_NAME_INDEX_MAP_OUPUT
        // ������
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

        // �� Bone�� Parent Index�� ������ �غ�
        AnimData.BoneParentIndexArray.resize(AnimData.BoneNameIndexMap.size(), -1);
        Matrix tr; 
        // �� Bone���� Parent���� Traverse �ϸ鼭 Assign��.
        processNodeRecursive(pScene->mRootNode, pScene, tr);

#pragma region DEBUGGING_2_PARENT_OUPUT
        // ������
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


        // ������� ���� AnimData�� BoneNameIndexMapping�� ������, Indexing ���� �۾����� �Ϸ��.
        // �ִϸ��̼� Ŭ�� ���� �б�
        // �̰��� ���� �ִϸ��̼� ���� �д� �κ�
        if (pScene->HasAnimations())
        {
            readAnimationClips(pScene);
        }

        // UpdateNormals(this->meshes); // Vertex Normal�� ���� ��� (�����)
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

            // �����
            // cout << "BoneMap " << count++ << " : " << bone->mName.C_Str()
            //     << " NumBoneWeights = " << bone->mNumWeights << endl;

            const uint32_t boneId = AnimData.BoneNameIndexMap[bone->mName.C_Str()];

            AnimData.OffsetMatrixArray[boneId] = Matrix((float*)&bone->mOffsetMatrix).Transpose();

            // �� ���� ������ �ִ� Vertex�� ����
            for (uint32_t j = 0; j < bone->mNumWeights; j++)
            {
                aiVertexWeight weight = bone->mWeights[j];
                assert(weight.mVertexId < boneIndices.size());
                boneIndices[weight.mVertexId].push_back(boneId);
                boneWeights[weight.mVertexId].push_back(weight.mWeight);
            }
        }

        // �������� Vertex �ϳ��� ������ �ִ� Bone�� �ִ� 4��
        // ������ �� ���� ���� �ִµ� �𵨸� ����Ʈ����� �����ϰų�
        // �о���̸鼭 weight�� �ʹ� ���� �͵��� �� ���� ����

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


// ���ؽ��� ������ ���������� �����ϴ� Bone���� ����� ����
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

                // bone�� �����Ǵ� node�� �̸��� ����
                // �ڿ��� node �̸����� �θ� ã�� �� ����
                // ������ ������ �� ������ BoneName�� ������� ����. ������ ������ �о���� ������ �����.
                AnimData.BoneNameIndexMap[pBone->mName.C_Str()] = -1;
                // ����: ���� ������ ������Ʈ ������ �ƴ�
                // ��Ÿ: bone->mWeights == 0�� ��쿡�� ���Խ�����
                // ��Ÿ: bone->mNode = 0�̶� ��� �Ұ�
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
        * �� ä���̶� �̸��� ���ǳİ�? �� ������ �������� ������ ä�η� ������ �Ǳ� ������.
        * �� ������ �������� �ð��� ���� ��ȭ ���.
        * �ϳ��� ��ȭ ��� �ϳ��� ä�ο��ٰ� �����͸� ���ش�.
        */
        for (uint32_t c = 0; c < pAnimation->mNumChannels; c++)
        {
            const aiNodeAnim* nodeAnim = pAnimation->mChannels[c];
            const int boneIndex = AnimData.BoneNameIndexMap[nodeAnim->mNodeName.C_Str()];
            clip.KeyBoneAndFrame2DArrays[boneIndex].resize(nodeAnim->mNumPositionKeys);

            // ��⼭ �ð��� ���� ������ ��� ���ϴ���, Assign ��.
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

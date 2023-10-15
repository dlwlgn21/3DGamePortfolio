#pragma once
#include <directxtk/SimpleMath.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace DirectX::SimpleMath;

namespace jh::graphics
{

struct AnimationClip
{
    struct Key
    {
        Vector3        Pos      =   Vector3(0.0f);
        Vector3        Scale    =   Vector3(1.0f);
        Quaternion     Rot      =   Quaternion();

        Matrix GetTransform()
        {
            return  Matrix::CreateScale(Scale) *
                    Matrix::CreateFromQuaternion(Rot) *
                    Matrix::CreateTranslation(Pos);
        }
    };

    std::string                             NameOfAnimationClip;                    // Name of this animation clip
    std::vector<std::vector<Key>>           KeyBoneAndFrame2DArrays;                // m_key[boneIdx][frameIdx]
    int                                     NumChannelsActuallyNumsBones = 0;       // Number of bones
    int                                     NumKeysActuallyNumsFrames = 0;          // Number of frames of this animation clip
    double                                  Duration = 0.0;                         // Duration of animation in ticks
    double                                  TicksPerSec = 0.0;                      // Frames per second

    const bool IsLastFrame(const int frame)
    {
        return KeyBoneAndFrame2DArrays[0].size() == frame ? true : false;
    }
};

struct AnimationData
{
    std::unordered_map<std::string, int32_t>        BoneNameIndexMap;               // 뼈 이름과 인덱스 정수
    std::vector<std::string>                        BoneIndexToNameArray;           // boneNameToId의 Id 순서대로 뼈 이름 저장
    std::vector<int32_t>                            BoneParentIndexArray;           // 부모 뼈의 인덱스
    std::vector<Matrix>                             OffsetMatrixArray;
    std::vector<Matrix>                             BoneTransformMatrixArray;
    std::vector<AnimationClip>                      ClipArray;
    Matrix                                          DefaultTransformMatrix;
    Matrix                                          RootTransformMatrix = Matrix();
    Matrix                                          AccumulatedRootTransformMatrix = Matrix();
    Vector3                                         PrevPos = Vector3(0.0f);

    Matrix GetFinalTransformMatrixRow(const int boneId, const int frame);
    void PrepareAllBoneTransformMatrices(const int clipId, const int frame);
};

}
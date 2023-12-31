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
        Vector3        Pos                  =   Vector3(0.0f);
        Vector3        Scale                =   Vector3(1.0f);
        Quaternion     Rot                  =   Quaternion();
        double          PositionTime        =   0.0;
        double          ScaleTime           =   0.0;
        double          RotationTime        =   0.0;

        Matrix GetTransform()
        {
            return  Matrix::CreateScale(Scale) *
                    Matrix::CreateFromQuaternion(Rot) *
                    Matrix::CreateTranslation(Pos);
        }
    };

    std::string                             NameOfAnimationClip;                    // Name of this animation clip
    std::vector<std::vector<Key>>           KeyBoneAndFrame2DArrays;                // m_key[boneIdx][frameIdx]
    int                                     NumChannelsActuallyNumsBones    = 0;    // Number of bones
    int                                     NumKeysActuallyNumsFrames       = 0;    // Number of frames of this animation clip
    double                                  Duration                        = 0.0;  // Duration of animation in ticks
    double                                  TicksPerSec                     = 0.0;  // Frames per second
    float                                   TotalAnimTime                   = 0.0f;
    const bool IsLastFrame(const float accumTime)
    {
        return accumTime >= TotalAnimTime ? true : false;
    }
};

struct AnimationData
{
    std::unordered_map<std::string, int32_t>        BoneNameIndexMap;               // �� �̸��� �ε��� ����
    std::vector<std::string>                        BoneIndexToNameArray;           // boneNameToId�� Id ������� �� �̸� ����
    std::vector<int32_t>                            BoneParentIndexArray;           // �θ� ���� �ε���
    std::vector<Matrix>                             OffsetMatrixArray;
    std::vector<Matrix>                             BoneTransformArray;
    std::unordered_map<std::string, AnimationClip>  ClipMap;
    Matrix                                          DefaultTransform;
    Matrix                                          RootTransform = Matrix();
    Matrix                                          AccumulatedRootTransform = Matrix();

    Matrix GetFinalTransformMatrixRow(const int boneId);
    void PrepareAllBoneTransforms(const std::string& clipName, const float accumTime);
};

}
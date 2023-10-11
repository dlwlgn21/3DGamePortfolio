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
};

struct AnimationData
{
    std::unordered_map<std::string, int32_t>        BoneNameIndexMap;               // �� �̸��� �ε��� ����
    std::vector<std::string>                        BoneIndexToNameArray;           // boneNameToId�� Id ������� �� �̸� ����
    std::vector<int32_t>                            BoneParentIndexArray;           // �θ� ���� �ε���
    std::vector<Matrix>                             OffsetMatrixArray;
    std::vector<Matrix>                             BoneTransformMatrixArray;
    std::vector<AnimationClip>                      ClipArray;
    Matrix                                          DefaultTransformMatrix;
    Matrix                                          RootTransformMatrix = Matrix();
    Matrix                                          AccumulatedRootTransformMatrix = Matrix();
    Vector3                                         PrevPos = Vector3(0.0f);

    // ���� ��ȯ��� 
    Matrix GetFinalTransformMatrix(int clipId, int boneId, int frame)
    {
        return DefaultTransformMatrix.Invert() * OffsetMatrixArray[boneId] * BoneTransformMatrixArray[boneId] * DefaultTransformMatrix;

        // defaultTransform�� ���� �о���϶� GeometryGenerator::Normalize() ���� ��� 
        // DefaultTransformMatrix.Invert() * offsetMatrices[boneId]�� �̸� ����ؼ� ��ġ�� 
        // DefaultTransformMatrix * rootTransform�� �̸� ����س��� ���� �ֽ��ϴ�. 
        // ����� ������ ������ ��ǥ�� ��ȯ ��ʷ� �����Ͻö�� ���ܳ��Ҵٰ� ��.
    }

    void UpdateBoneTransformMatrices(int clipId, int frame)
    {
        auto& clip = ClipArray[clipId];

        // Matrix�� ���ϴ� ������.
        // ��, M i = M i * M i-1 ... M 1 * M 0 ���� ������ �ݺ��ϴ� ��.
        // clip��������, ������ bone�� ���ؼ� �ݺ����� �Ҿƾ� ��.
        for (int boneIndex = 0; boneIndex < BoneTransformMatrixArray.size(); boneIndex++)
        {
            // keys�� boneID�� �ε��� �ϸ�, �������� �ε��� �� �� ����.
            std::vector<AnimationClip::Key>& keys = clip.KeyBoneAndFrame2DArrays[boneIndex];

            // ����: ��� ä��(��)�� frame ������ �������� ����

            const int parentIdx = BoneParentIndexArray[boneIndex];
            const Matrix parentMatrix =
                parentIdx >= 0 ? BoneTransformMatrixArray[parentIdx] : AccumulatedRootTransformMatrix;

            // keys.size()�� 0�� ��쿡�� Identity ��ȯ
            // �� ��쿡�� �� key�� ���ؼ��� AnimationData�� ������ �̾߱���. �����ֵ�. �̷� �̾߱�.
            AnimationClip::Key key =
                keys.size() > 0 ? keys[frame % keys.size()] : AnimationClip::Key(); // key�� reference �ƴ�

            // Root�� ���
            if (parentIdx < 0)
            {
                if (frame != 0)
                {
                    AccumulatedRootTransformMatrix = Matrix::CreateTranslation(key.Pos - PrevPos) * AccumulatedRootTransformMatrix;
                }
                else
                {
                    Vector3 temp = AccumulatedRootTransformMatrix.Translation();
                    temp.y = key.Pos.y; // ���� ���⸸ ù ���������� ����
                    AccumulatedRootTransformMatrix.Translation(temp);
                }

                PrevPos = key.Pos;
                key.Pos = Vector3(0.0f); // ��ſ� �̵� ���
            }
            BoneTransformMatrixArray[boneIndex] = key.GetTransform() * parentMatrix;
        }
    }
};

}
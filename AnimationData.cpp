#include "AnimationData.h"
#include <algorithm>

using namespace DirectX::SimpleMath;

namespace jh::graphics
{

Matrix AnimationData::GetFinalTransformMatrixRow(const int boneId)
{
    return DefaultTransform.Invert() * OffsetMatrixArray[boneId] * BoneTransformArray[boneId] * DefaultTransform;

    // defaultTransform�� ���� �о���϶� GeometryGenerator::Normalize() ���� ��� 
    // DefaultTransformMatrix.Invert() * offsetMatrices[boneId]�� �̸� ����ؼ� ��ġ�� 
    // DefaultTransformMatrix * rootTransform�� �̸� ����س��� ���� �ֽ��ϴ�. 
    // ����� ������ ������ ��ǥ�� ��ȯ ��ʷ� �����Ͻö�� ���ܳ��Ҵٰ� ��.
}

void AnimationData::PrepareAllBoneTransforms(const std::string& clipNameKey, const float accumTime)
{
    assert(clipNameKey != "");
    auto iter = ClipMap.find(clipNameKey);
    if (iter != ClipMap.end())
    {
        auto& clip = iter->second;
        for (int boneIndex = 0; boneIndex < BoneTransformArray.size(); boneIndex++)
        {
            // keys�� boneID�� �ε��� �ϸ�, �������� �ε��� �� �� ����.
            std::vector<AnimationClip::Key>& keys = clip.KeyBoneAndFrame2DArrays[boneIndex];

            // ����: ��� ä��(��)�� frame ������ �������� ����
            const int parentIdx = BoneParentIndexArray[boneIndex];
            const Matrix parentMatrix = parentIdx >= 0 ? BoneTransformArray[parentIdx] : AccumulatedRootTransform;

            AnimationClip::Key key;
            if (keys.size() == 0)
            {
                key = AnimationClip::Key();
            }
            else if (keys.size() == 1)
            {
                key = keys[0];
            }
            else
            {
                UINT frameIndex = 0;
                for (int i = 0; i < keys.size() - 1; ++i)
                {
                    assert(i + 1 < keys.size());
                    float nextTime = static_cast<float>(keys[i + 1].PositionTime);
                    if (accumTime < nextTime)
                    {
                        frameIndex = i;
                        break;
                    }
                }
                UINT nextFrameIndex = frameIndex + 1;
                assert(frameIndex < keys.size());
                float t1 = (float)keys[frameIndex].PositionTime;
                float t2 = (float)keys[nextFrameIndex].PositionTime;
                float deltaTime = t2 - t1;
                float factor = (accumTime - t1) / deltaTime;
                factor = std::clamp(factor, 0.0f, 1.0f);

                Vector3 lerpPos = Vector3::Lerp(keys[frameIndex].Pos, keys[nextFrameIndex].Pos, factor);
                Quaternion lerpRot = Quaternion::Slerp(keys[frameIndex].Rot, keys[nextFrameIndex].Rot, factor);
                Vector3 lerpScale = Vector3::Lerp(keys[frameIndex].Scale, keys[nextFrameIndex].Scale, factor);
                key.Pos = lerpPos;
                key.Scale = lerpScale;
                key.Rot = lerpRot;
                key.PositionTime = keys[frameIndex].PositionTime;
                key.RotationTime = keys[frameIndex].RotationTime;
                key.ScaleTime = keys[frameIndex].ScaleTime;
            }
            BoneTransformArray[boneIndex] = key.GetTransform() * parentMatrix;
        }
    }
    else
    {
        assert(false);
    }
}
}
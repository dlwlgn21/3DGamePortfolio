#include "AnimationData.h"
#include <algorithm>

using namespace DirectX::SimpleMath;

namespace jh::graphics
{

Matrix AnimationData::GetFinalTransformMatrixRow(const int boneId)
{
    return DefaultTransform.Invert() * OffsetMatrixArray[boneId] * BoneTransformArray[boneId] * DefaultTransform;

    // defaultTransform은 모델을 읽어들일때 GeometryGenerator::Normalize() 에서 계산 
    // DefaultTransformMatrix.Invert() * offsetMatrices[boneId]를 미리 계산해서 합치고 
    // DefaultTransformMatrix * rootTransform을 미리 계산해놓을 수도 있습니다. 
    // 여기는 교육용 예제라서 좌표계 변환 사례로 참고하시라고 남겨놓았다고 함.
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
            // keys의 boneID를 인덱싱 하면, 프레임을 인덱싱 할 수 있음.
            std::vector<AnimationClip::Key>& keys = clip.KeyBoneAndFrame2DArrays[boneIndex];

            // 주의: 모든 채널(뼈)이 frame 개수가 동일하진 않음
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
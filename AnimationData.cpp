#include "AnimationData.h"
using namespace DirectX::SimpleMath;

namespace jh::graphics
{

Matrix AnimationData::GetFinalTransformMatrixRow(const int boneId, const int frame)
{
    return DefaultTransformMatrix.Invert() * OffsetMatrixArray[boneId] * BoneTransformMatrixArray[boneId] * DefaultTransformMatrix;

    // defaultTransform은 모델을 읽어들일때 GeometryGenerator::Normalize() 에서 계산 
    // DefaultTransformMatrix.Invert() * offsetMatrices[boneId]를 미리 계산해서 합치고 
    // DefaultTransformMatrix * rootTransform을 미리 계산해놓을 수도 있습니다. 
    // 여기는 교육용 예제라서 좌표계 변환 사례로 참고하시라고 남겨놓았다고 함.
}

void AnimationData::PrepareAllBoneTransformMatrices(const std::string& clipNameKey, const int frame)
{
    assert(clipNameKey != "");
    auto iter = ClipMap.find(clipNameKey);
    if (iter != ClipMap.end())
    {
        auto& clip = iter->second;
        for (int boneIndex = 0; boneIndex < BoneTransformMatrixArray.size(); boneIndex++)
        {
            // keys의 boneID를 인덱싱 하면, 프레임을 인덱싱 할 수 있음.
            std::vector<AnimationClip::Key>& keys = clip.KeyBoneAndFrame2DArrays[boneIndex];

            // 주의: 모든 채널(뼈)이 frame 개수가 동일하진 않음

            const int parentIdx = BoneParentIndexArray[boneIndex];
            const Matrix parentMatrix = parentIdx >= 0 ? BoneTransformMatrixArray[parentIdx] : AccumulatedRootTransformMatrix;

            // keys.size()가 0일 경우에는 Identity 변환
            // 이 경우에는 그 key에 대해서는 AnimationData가 없었던 이야기임. 멈춰있따. 이런 이야기.
            AnimationClip::Key key = keys.size() > 0 ? keys[frame % keys.size()] : AnimationClip::Key(); // key가 reference 아님

            // Root일 경우
            if (parentIdx < 0)
            {
                if (frame != 0)
                {
                    //AccumulatedRootTransformMatrix = Matrix::CreateTranslation(key.Pos - PrevPos) * AccumulatedRootTransformMatrix;
                }
                else
                {
                    Vector3 temp = AccumulatedRootTransformMatrix.Translation();
                    temp.y = key.Pos.y; // 높이 방향만 첫 프레임으로 보정
                    AccumulatedRootTransformMatrix.Translation(temp);
                }
                //PrevPos = key.Pos;
                //key.Pos = Vector3(0.0f);
            }
            BoneTransformMatrixArray[boneIndex] = key.GetTransform() * parentMatrix;
        }
    }
    else
    {
        assert(false);
    }
}
}
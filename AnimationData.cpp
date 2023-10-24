#include "AnimationData.h"
using namespace DirectX::SimpleMath;

namespace jh::graphics
{

Matrix AnimationData::GetFinalTransformMatrixRow(const int boneId, const int frame)
{
    return DefaultTransformMatrix.Invert() * OffsetMatrixArray[boneId] * BoneTransformMatrixArray[boneId] * DefaultTransformMatrix;

    // defaultTransform�� ���� �о���϶� GeometryGenerator::Normalize() ���� ��� 
    // DefaultTransformMatrix.Invert() * offsetMatrices[boneId]�� �̸� ����ؼ� ��ġ�� 
    // DefaultTransformMatrix * rootTransform�� �̸� ����س��� ���� �ֽ��ϴ�. 
    // ����� ������ ������ ��ǥ�� ��ȯ ��ʷ� �����Ͻö�� ���ܳ��Ҵٰ� ��.
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
            // keys�� boneID�� �ε��� �ϸ�, �������� �ε��� �� �� ����.
            std::vector<AnimationClip::Key>& keys = clip.KeyBoneAndFrame2DArrays[boneIndex];

            // ����: ��� ä��(��)�� frame ������ �������� ����

            const int parentIdx = BoneParentIndexArray[boneIndex];
            const Matrix parentMatrix = parentIdx >= 0 ? BoneTransformMatrixArray[parentIdx] : AccumulatedRootTransformMatrix;

            // keys.size()�� 0�� ��쿡�� Identity ��ȯ
            // �� ��쿡�� �� key�� ���ؼ��� AnimationData�� ������ �̾߱���. �����ֵ�. �̷� �̾߱�.
            AnimationClip::Key key = keys.size() > 0 ? keys[frame % keys.size()] : AnimationClip::Key(); // key�� reference �ƴ�

            // Root�� ���
            if (parentIdx < 0)
            {
                if (frame != 0)
                {
                    //AccumulatedRootTransformMatrix = Matrix::CreateTranslation(key.Pos - PrevPos) * AccumulatedRootTransformMatrix;
                }
                else
                {
                    Vector3 temp = AccumulatedRootTransformMatrix.Translation();
                    temp.y = key.Pos.y; // ���� ���⸸ ù ���������� ����
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
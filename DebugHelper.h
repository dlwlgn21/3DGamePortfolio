#pragma once
#include <directxtk/SimpleMath.h>


namespace jh
{

class DebugHelper final
{
public:
	static void PrintVector(const DirectX::SimpleMath::Vector3& vector);
	static void PrintMatrix(const DirectX::SimpleMath::Matrix& matrixRowMajor);
};

}
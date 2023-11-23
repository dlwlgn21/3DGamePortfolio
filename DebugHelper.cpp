#include "DebugHelper.h"
#include <iostream>

namespace jh
{
	void DebugHelper::PrintVector(const DirectX::SimpleMath::Vector3& vector)
	{
		std::cout << "Vector : " << vector.x << ", " << vector.y << ", " << vector.z << std::endl;
	}
	void DebugHelper::PrintMatrix(const DirectX::SimpleMath::Matrix& matrixRowMajor)
	{
		std::cout << "Print Matrix\n"
			<< matrixRowMajor.m[0][0] << ", " << matrixRowMajor.m[0][1] << ", " << matrixRowMajor.m[0][2] << ", " << matrixRowMajor.m[0][3] << "\n"
			<< matrixRowMajor.m[1][0] << ", " << matrixRowMajor.m[1][1] << ", " << matrixRowMajor.m[1][2] << ", " << matrixRowMajor.m[1][3] << "\n"
			<< matrixRowMajor.m[2][0] << ", " << matrixRowMajor.m[2][1] << ", " << matrixRowMajor.m[2][2] << ", " << matrixRowMajor.m[2][3] << "\n"
			<< matrixRowMajor.m[3][0] << ", " << matrixRowMajor.m[3][1] << ", " << matrixRowMajor.m[3][2] << ", " << matrixRowMajor.m[3][3] << std::endl;
	} 
}
#include "GausianFilter.h"

#include <valarray>

using namespace std;

void GausianFilter::CreateKernel(float** Kernel, int Size, float Sigma)
{
	float Sum = 0;
	float KernelRadius = (Size - 1) / 2;
	for (int i = 0; i < Size; i++)
		for (int j = 0; j < Size; j++)
		{
			double x = Gausian(i, KernelRadius, Sigma)
				* Gausian(j, KernelRadius, Sigma);
			Kernel[i][j] = x;
			Sum += x;
		}
	for (int i = 0; i < Size; i++)
		for (int j = 0; j < Size; j++)
			Kernel[i][j] /= Sum;
}

void GausianFilter::SmoothMap(float** Map, int MapSize, float** FinalMap, float** Kernel)
{
	for (int i = 1; i < MapSize - 1; i++)
		for (int j = 1; j < MapSize - 1; j++)
		{
			float LeftTop = Map[i - 1][j - 1] * Kernel[0][0];
			float LeftCenter = Map[i][j - 1] * Kernel[1][0];
			float LeftBottom = Map[i + 1][j - 1] * Kernel[2][0];
			float CenterTop = Map[i - 1][j] * Kernel[0][1];
			float Center = Map[i][j] * Kernel[1][1];
			float CenterBottom = Map[i + 1][j] * Kernel[2][1];
			float RightTop = Map[i - 1][j + 1] * Kernel[0][2];
			float RightCenter = Map[i][j + 1] * Kernel[1][2];
			float RightBottom = Map[i + 1][j + 1] * Kernel[2][2];
			FinalMap[i][j] = (LeftTop + LeftCenter + LeftBottom + CenterBottom + CenterTop + CenterBottom + RightTop +
				RightCenter + Center + RightBottom) / 9;
		}
}


float GausianFilter::Gausian(float X, float Mu, float Sigma)
{
	const float A = (X - Mu) / Sigma;
	return std::exp(-0.5 * A * A);
}

#include "GausianFilter.h"

#include <valarray>

using namespace std;
void GausianFilter::CreateKernel(float** Kernel, int Size, float Sigma)
{
	float Sum = 0;
	float KernelRadius = (Size-1)/2;
	for (int i = 0; i < Size; i++)
		for (int j = 0; j < Size; j++) {
			double x = Gausian(i, KernelRadius, Sigma)
					* Gausian(j, KernelRadius, Sigma);
			Kernel[i][j] = x;
			Sum += x;
		}
	for (int i = 0; i < Size; i++)
		for (int j = 0; j < Size; j++)
			Kernel[i][j] /= Sum;
}

float GausianFilter::Gausian(float X, float Mu, float Sigma)
{
	const float A = ( X - Mu ) / Sigma;
	return std::exp( -0.5 * A * A );
}

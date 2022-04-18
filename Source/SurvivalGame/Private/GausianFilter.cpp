#include "GausianFilter.h"

void GausianFilter::CreateKernel(float** Kernel, int Size, float Sigma)
{
	// float r, s = 2.0 * math;
 //
	// // sum is for normalization
	// double sum = 0.0;
 //
	// // generating 5x5 kernel
	// for (int x = -2; x <= 2; x++) {
	// 	for (int y = -2; y <= 2; y++) {
	// 		r = sqrt(x * x + y * y);
	// 		GKernel[x + 2][y + 2] = (exp(-(r * r) / s)) / (M_PI * s);
	// 		sum += GKernel[x + 2][y + 2];
	// 	}
	// }
 //
	// // normalising the Kernel
	// for (int i = 0; i < 5; ++i)
	// 	for (int j = 0; j < 5; ++j)
	// 		GKernel[i][j] /= sum;
}

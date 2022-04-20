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
		{
			Kernel[i][j] /= Sum;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%f"),Kernel[i][j]));
		}
	
}


float GausianFilter::Gausian(float X, float Mu, float Sigma)
{
	const float A = (X - Mu) / Sigma;
	return std::exp(-0.5 * A * A);
}

void GausianFilter::SmoothMap(float** Map, int MapSize, float** FinalMap, float** Kernel,int KernelSize)
{
	float** PixelMap = new float*[KernelSize];
	for(int i=0;i<KernelSize;i++)
	{
		PixelMap[i] = new float[KernelSize];
	}
	const int Border = (KernelSize-1)/2;
	for (int i = Border; i < MapSize - Border; i++)
		for (int j = Border; j < MapSize - Border; j++)
		{
			for(int k = 0;k<KernelSize;k++)
			{
				for(int l = 0;l<KernelSize;l++)
				{
					PixelMap[k][l] = Map[i+k-Border][j+l-Border]*Kernel[0][1];
				}
			}
			float Sum = 0;
			for(int k=0;k<KernelSize;k++)
				for(int l=0;l<KernelSize;l++)
					Sum+=PixelMap[k][l];
			FinalMap[i][j] = Sum / (KernelSize*KernelSize);
		}
}


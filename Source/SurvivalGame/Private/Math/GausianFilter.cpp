#include "Math/GausianFilter.h"

#include <valarray>

#include "Math/SupportMethods.h"

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
		}
		//SupportMethods::PrintMass(Kernel,Size);
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
	for (int i = 0; i < MapSize; i++)
		for (int j = 0; j < MapSize; j++)
		{
			for(int k = 0;k<KernelSize;k++)
			{
				for(int l = 0;l<KernelSize;l++)
				{
					int XIndex =i+k-Border;
					int YIndex = j+l-Border;
					if((XIndex>0) & (XIndex<MapSize) & (YIndex>0) & (YIndex<MapSize))
					{
						PixelMap[k][l] = Map[i+k-Border][j+l-Border]*Kernel[k][l];
					}else
					{
						PixelMap[k][l]=-1;
					}
				}
			}
			float Sum = 0;
			for(int k=0;k<KernelSize;k++)
				for(int l=0;l<KernelSize;l++)
					Sum+=PixelMap[k][l]<0?0:PixelMap[k][l];
			FinalMap[i][j] = Sum;
		}
}


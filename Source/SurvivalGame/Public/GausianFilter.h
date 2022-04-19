#pragma once

class GausianFilter
{
public:
	static void CreateKernel(float** Kernel, int Size, float Sigma);
	static void SmoothMap(float** Map, int MapSize, float** FinalMap, float** Kernel);
private:
	static float Gausian(float X, float Mu, float Sigma);
};

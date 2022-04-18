#pragma once

class GausianFilter
{
public:
	static void CreateKernel(float** Kernel,int Size,float Sigma);
private:
	static float Gausian(float X,float Mu,float Sigma);
};

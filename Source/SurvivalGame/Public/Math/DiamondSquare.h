#pragma once

class DiamondSquare
{
public:
	static void GenerateMap(float** Map,int Size,float RoughnessFactorParam);
private:
	static void DiamondAlgorithm(float** Map,int Size,int MapSize,int Iteration,float Roughness);
	static void InitializeCorners(float** Map,int Size);
	static void DiamondStep(float** Map,int X,int Y,int Reach,int Iteration,int MapSize,float Roughness);
	static void SquareStep(float** Map,int X,int Y,int Reach,int Iteration,int MapSize,float Roughness);
	static float RandomValue(float Min,float Max);
};

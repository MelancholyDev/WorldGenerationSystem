#pragma once

class DiamondSquare
{
public:
	void GenerateMap(float** Map,int Size,float RoughnessFactorParam,int Seed);
private:
	void DiamondAlgorithm(float** Map,int Size,int MapSize,int Iteration,float Roughness);
	void InitializeCorners(float** Map,int Size);
	void SquareStep(float** Map,int X,int Y,int Reach,int Iteration,int MapSize,float Roughness);
	void DiamondStep(float** Map,int X,int Y,int Reach,int Iteration,int MapSize,float Roughness);

	float CalculateFinalV1(float Avg,float Roughness);
	float CalculateFinalV2(float Avg,float Roughness,float Iteration);
	float RandomValue(float Min,float Max);
};

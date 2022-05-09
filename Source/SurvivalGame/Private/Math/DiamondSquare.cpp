#include "Math/DiamondSquare.h"

void DiamondSquare::GenerateMap(float** Map, int Size, float Roughness, int Seed)
{
	FMath::SRandInit(Seed);
	for (int i = 0; i < Size; i++)
		for (int j = 0; j < Size; j++)
		{
			Map[i][j] = 0;
		}
	InitializeCorners(Map, Size);
	DiamondAlgorithm(Map, Size - 1, Size, 1, Roughness);
}

void DiamondSquare::InitializeCorners(float** Map, int Size)
{
	int Shift = Size - 1;
	for (int i = 0; i < Size; i += Shift)
		for (int j = 0; j < Size; j += Shift)
			Map[i][j] = 0;
}

void DiamondSquare::DiamondAlgorithm(float** Map, int Size, int MapSize, int Iteration, float Roughness)
{
	if (Size < 2)
	{
		return;
	}
	const int Half = Size / 2;
	for (int i = Half; i < MapSize; i += Size)
		for (int j = Half; j < MapSize; j += Size)
		{
			DiamondStep(Map, i, j, Half, Iteration, MapSize, Roughness);
		}
	int Column = 0;
	for (int i = 0; i < MapSize; i += Half)
	{
		if (Column % 2 == 0)
			for (int j = Half; j < MapSize; j += Size)
				SquareStep(Map, i, j, Half, Iteration, MapSize, Roughness);
		else
			for (int j = 0; j < MapSize; j += Size)
				SquareStep(Map, i, j, Half, Iteration, MapSize, Roughness);
		Column++;
	}
	DiamondAlgorithm(Map, Half, MapSize, Iteration + 1, Roughness );
}

void DiamondSquare::SquareStep(float** Map, int X, int Y, int Reach, int Iteration, int MapSize, float Roughness)
{
	float Sum = 0;
	
	Sum += Map[(X - Reach + MapSize - 1) % (MapSize - 1)][Y];
	Sum += Map[(X + Reach) % (MapSize-1)][Y];
	Sum += Map[X][(Y - Reach + MapSize - 1) % (MapSize - 1)];
	Sum += Map[X][(Y + Reach) % (MapSize-1)];
	
	const float Avg = Sum / 4;
	const float RandomAdd = RandomValue(-FMath::Pow(Roughness,Iteration), FMath::Pow(Roughness,Iteration));
	//Map[X][Y] = Avg + (RandomAdd * 2 * Roughness) - Roughness;
	Map[X][Y] = Avg + RandomAdd;
}

void DiamondSquare::DiamondStep(float** Map, int X, int Y, int Reach, int Iteration, int MapSize, float Roughness)
{
	float Sum = 0;
	
	Sum += Map[X - Reach][Y - Reach];
	Sum += Map[X - Reach][Y + Reach];
	Sum += Map[X + Reach][Y - Reach];
	Sum += Map[X + Reach][Y + Reach];
	
	const float Avg = Sum / 4;
	//const float RandomAdd = RandomValue(0, 1);
	const float RandomAdd = RandomValue(-FMath::Pow(Roughness,Iteration), FMath::Pow(Roughness,Iteration));
	//Map[X][Y] = Avg + (RandomAdd * 2 * Roughness) - Roughness;
	Map[X][Y] = Avg + RandomAdd;
}

float DiamondSquare::CalculateFinalV1(float Avg, float Roughness)
{
	const float RandomAdd = RandomValue(0, 1);
	return Avg + (RandomAdd * 2 * Roughness) - Roughness;
}

float DiamondSquare::CalculateFinalV2(float Avg, float Roughness, float Iteration)
{
	const float RandomAdd = RandomValue(-FMath::Pow(Roughness,Iteration), FMath::Pow(Roughness,Iteration));
	return Avg+RandomAdd;
}

float DiamondSquare::RandomValue(float Min, float Max)
{
	const float Diff = Max - Min;
	const float Multiplier = FMath::SRand();
	return Min + Diff * Multiplier;
}

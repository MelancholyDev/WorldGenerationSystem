#include "Math/DiamondSquare.h"

#include "TestDiamond/DiamondSquareMapGeneration.h"

void DiamondSquare::GenerateMap(float** Map, int Size, float Roughness)
{
	InitializeCorners(Map, Size);
	DiamondAlgorithm(Map, Size-1,Size,1,Roughness);
}

void DiamondSquare::InitializeCorners(float** Map, int Size)
{
	int Shift = Size - 1;
	for (int i = 0; i < Size; i += Shift)
		for (int j = 0; j < Size; j += Shift)
			//Map[i][j] = FMath::RandRange(.0f, 1.0f);
			Map[i][j] =0;
}

void DiamondSquare::DiamondAlgorithm(float** Map, int Size,int MapSize,int Iteration,float Roughness)
{
	int Half = Size / 2;
	if (Half < 1)
	{
		return;
	}
	for (int i = Half; i < MapSize; i += Size)
		for (int j = Half; j < MapSize; j += Size)
		{
			SquareStep(Map,i,j,Half,Iteration,MapSize,Roughness);
		}
	int Column = 0;
	for (int i = 0; i < MapSize; i += Half)
	{
		if (Column % 2 == 0)
			for (int j = Half; j < MapSize; j += Size)
				DiamondStep(Map, i, j, Half,Iteration,MapSize,Roughness);
		else
			for (int j = 0; j < MapSize; j += Size)
				DiamondStep(Map, i, j, Half,Iteration,MapSize,Roughness);
		Column++;
	}
	DiamondAlgorithm(Map,Half,MapSize,Iteration+1,Roughness/2);
}

void DiamondSquare::DiamondStep(float** Map, int X,int Y,int Reach,int Iteration,int MapSize,float Roughness)
{
	int Count = 0;
	float Sum=0;
	if (X - Reach >= 0)
	{
		Sum += Map[X - Reach][Y];
		Count++;
	}
	if (X + Reach <MapSize)
	{
		Sum += Map[X + Reach][Y];
		Count++;
	}
	if (Y - Reach >= 0)
	{
		Sum = Map[X][Y-Reach];
		Count++;
	}
	if (Y + Reach< MapSize)
	{
		Sum += Map[X][Y+Reach];
		Count++;
	}
	float Avg=Sum/Count;
	Map[X][Y]=Avg+(RandomValue(Iteration,Roughness)*2*Roughness)-Roughness;
}

void DiamondSquare::SquareStep(float** Map, int X,int Y,int Reach,int Iteration,int MapSize,float Roughness)
{
	int Count = 0;
	float Sum=0;
	if (X - Reach >= 0 && Y - Reach >= 0)
	{
		Sum += Map[X - Reach][Y - Reach];
		Count++;
	}
	if (X - Reach >= 0 && Y + Reach < MapSize)
	{
		Sum += Map[X - Reach][Y + Reach];
		Count++;
	}
	if (X + Reach < MapSize && Y - Reach >= 0)
	{
		Sum = Map[X+Reach][Y-Reach];
		Count++;
	}
	if (X + Reach < MapSize && Y + Reach< MapSize)
	{
		Sum += Map[X+Reach][Y+Reach];
		Count++;
	}
	float Avg=Sum/Count;
	Map[X][Y]=Avg+(RandomValue(Iteration,Roughness)*2*Roughness)-Roughness;
}

float DiamondSquare::RandomValue(float Min,float Max)
{
	return FMath::RandRange(Min,Max);
}

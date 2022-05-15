#include "Math/PerlinWormGenerator.h"

#include "Math/PerlinWorm.h"

PerlinWormGenerator::PerlinWormGenerator(int SizeParam, int DepthParam, FWormSettings WormSettingsParam)
{
	InitializeDirectionList();
	WormSettings = WormSettingsParam;
	Depth = DepthParam;
	Size = SizeParam;
}

void PerlinWormGenerator::InitializeDirectionList()
{
	Directions = new TArray<FIntVector>();
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			for (int k = -1; k <= 1; k++)
			{
				if ((i != 0) | (j != 0) | (k != 0))
				{
					Directions->Add(FIntVector(i, j, k));
				}
			}
}

bool PerlinWormGenerator::CheckNeighbours(int X, int Y, int Z, bool IsMaxima)
{
	for (int i = 0; i < Directions->Num(); i++)
	{
		FIntVector Direction = (*Directions)[i];
		int X_Check = X + Direction.X;
		int Y_Check = Y + Direction.Y;
		int Z_Check = Z + Direction.Z;
		float FirstNoise = UndergroundMap[X][Y][Z];
		if (!((X_Check < 0 || Y_Check < 0 || Z_Check < 0) || (X_Check >= Size || Y_Check >= Size || Z_Check >= Depth)))
		{
			float NeigbourNoise = UndergroundMap[X_Check][Y_Check][Z_Check];
			if (FailCondition(FirstNoise, NeigbourNoise, IsMaxima))
			{
				return false;
			}
		}
	}
	return true;
}

FVector PerlinWormGenerator::GetPerlinWormDirection()
{
	return FVector();
}

void PerlinWormGenerator::FindLocalMaximas()
{
	Maximas = new TArray<FIntVector>();
	for (int X = 0; X < Size; X++)
	{
		for (int Y = 0; Y < Size; Y++)
		{
			for (int Z = 0; Z < Depth; Z++)
			{
				if (CheckNeighbours(X, Y, Z, true))
				{
					Maximas->Add(FIntVector(X, Y, Z));
				}
			}
		}
	}
}

void PerlinWormGenerator::FindLocalMinimas()
{
	Minimas = new TArray<FIntVector>();
	for (int X = 0; X < Size; X++)
	{
		for (int Y = 0; Y < Size; Y++)
		{
			for (int Z = 0; Z < Depth; Z++)
			{
				if (CheckNeighbours(X, Y, Z, false))
				{
					Minimas->Add(FIntVector(X, Y, Z));
				}
			}
		}
	}
}

bool PerlinWormGenerator::FailCondition(float FirstNoise, float NeighbourNoise, bool IsMaxima)
{
	if (IsMaxima)
	{
		return NeighbourNoise > FirstNoise;
	}
	else
	{
		return NeighbourNoise < FirstNoise;
	}
}

void PerlinWormGenerator::CreateWorm(FIntVector Maxim)
{
	PerlinWorm* Worm = new PerlinWorm(UndergroundMap,TempMap,WormSettings, Maxim);
}

void PerlinWormGenerator::GenerateCaves(float*** UndergroundMapParam)
{
	UndergroundMap = UndergroundMapParam;
	TempMap = new float**[Size];
	for (int i = 0; i < Size; i++)
	{
		TempMap[i] = new float*[Size];
		for (int j = 0; j < Size; j++)
			TempMap[i][j] = new float[Depth];
	}
	for (int i = 0; i < Size; i++)
		for (int j = 0; j < Size; j++)
			for (int k = 0; k < Size; k++)
			TempMap[i][j][k] = 1;
	
	FindLocalMaximas();
	FindLocalMinimas();
	for (int i = 0; i < Maximas->Num(); i++)
	{
		FIntVector Maxim = (*Maximas)[i];
		CreateWorm(Maxim);
	}
}

#pragma once
#include <list>

class PerlinWormGenerator
{
public:
	PerlinWormGenerator(float*** UndergroundMapParam, int SizeParam, int DepthParam);
	void GenerateCaves();
private:
	void InitializeDirectionList();
	bool CheckNeighbours(int X, int Y, int Z, bool IsMaxima);
	FVector GetPerlinWormDirection();
	void FindLocalMaximas();
	void FindLocalMinimas();
	bool FailCondition(float FirstNoise, float NeighbourNoise, bool IsMaxima);
	void CreateWorm(FIntVector Maxim);


	float*** UndergroundMap;
	TArray<FIntVector>* Directions;
	TArray<FIntVector>* Maximas;
	TArray<FIntVector>* Minimas;
	int Size;
	int Depth;
};

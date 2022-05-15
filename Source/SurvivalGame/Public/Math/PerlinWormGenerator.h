#pragma once
#include <list>

#include "Structures/FWormSettings.h"

class PerlinWormGenerator
{
public:
	PerlinWormGenerator(int SizeParam, int DepthParam,FWormSettings WormSettingsParam);
	void GenerateCaves(float*** UndergroundMapParam);
private:
	void InitializeDirectionList();
	bool CheckNeighbours(int X, int Y, int Z, bool IsMaxima);
	FVector GetPerlinWormDirection();
	void FindLocalMaximas();
	void FindLocalMinimas();
	bool FailCondition(float FirstNoise, float NeighbourNoise, bool IsMaxima);
	void CreateWorm(FIntVector Maxim);


	float*** UndergroundMap;
	float*** TempMap;
	TArray<FIntVector>* Directions;
	TArray<FIntVector>* Maximas;
	TArray<FIntVector>* Minimas;
	int Size;
	int Depth;
	FWormSettings WormSettings;
};

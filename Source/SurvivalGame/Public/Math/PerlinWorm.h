#pragma once
#include "Structures/FWormSettings.h"

class PerlinWorm
{
public:
	PerlinWorm(float*** UndergroundMapParam, float*** FirstNoiseParam,float*** SecondNoiseParam, FWormSettings WormSettingsParam,
	           FIntVector StartPositionPram, int SizeParam, int DepthParam);
	void MoveLength(int Length);
private:
	FIntVector CurrentDirection;
	FIntVector CurrentPosition;
	FIntVector ConvergancePoint;
	bool MoveToConvergancePoint;
	float*** UndergroundMap;
	float*** FirstNoise;
	float*** SecondNoise;
	int Size;
	int Depth;
	int Width;
	int UniqueBlocks;
	FWormSettings WormSetings;

	void Move();
	void MarkEmpty(int X,int Y,int Z);
	FIntVector GetDirection();
};

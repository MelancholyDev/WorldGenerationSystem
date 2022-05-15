#pragma once
#include "Structures/FWormSettings.h"

class PerlinWorm
{
public:
	PerlinWorm(float*** UndergroundMapParam, float*** TempMapParam, FWormSettings WormSettingsParam,
	           FIntVector StartPositionPram, int SizeParam, int DepthParam, int WidthParam);
	void MoveLength(int Length);
private:
	FIntVector CurrentDirection;
	FIntVector CurrentPosition;
	FIntVector ConvergancePoint;
	bool MoveToConvergancePoint;
	float*** UndergroundMap;
	float*** TempMap;
	int Size;
	int Depth;
	int Width;
	FWormSettings WormSetings;

	void Move();
	void MarkEmpty(int X,int Y,int Z);
	FIntVector GetDirection();
};

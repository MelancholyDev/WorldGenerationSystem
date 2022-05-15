#pragma once
#include "Structures/FWormSettings.h"

class PerlinWorm
{
public:
	PerlinWorm(float*** UndergroundMapParam,float*** TempMapParam,FWormSettings WormSettingsParam, FIntVector StartPositionPram);
private:
	FIntVector CurrentDirection;
	FIntVector CurrentPosition;
	FIntVector ConvergancePoint;
	bool MoveToConvergancePoint;
	float*** UndergroundMap;
	float*** TempMap;
	void MoveLength(int Length);
	void Move();
	FIntVector GetDirection();
	FWormSettings WormSetings;
};

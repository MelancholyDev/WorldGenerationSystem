#pragma once
#include "Structures/FWormSettings.h"

class PerlinWorm
{
public:
	PerlinWorm();
	PerlinWorm(FWormSettings WormSettingsParam, FIntVector StartPositionPram, FIntVector ConvergancePointParam);
	PerlinWorm(FWormSettings WormSettingsParam, FIntVector StartPositionPram);
private:
	FIntVector CurrentDirection;
	FIntVector CurrentPosition;
	FIntVector ConvergancePoint;
	bool MoveToConvergancePoint;

	FWormSettings WormSetings;
};

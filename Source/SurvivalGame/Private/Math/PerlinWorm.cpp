#include "Math/PerlinWorm.h"

PerlinWorm::PerlinWorm()
{
}

PerlinWorm::PerlinWorm(FWormSettings WormSettingsParam, FIntVector StartPositionParam, FIntVector ConvergancePointParam)
{
	WormSetings=WormSettingsParam;
	CurrentPosition = StartPositionParam;
	ConvergancePoint=ConvergancePointParam;
	MoveToConvergancePoint=true;;
}

PerlinWorm::PerlinWorm(FWormSettings WormSettingsParam, FIntVector StartPositionParam)
{
	WormSetings=WormSettingsParam;
	CurrentPosition = StartPositionParam;
	MoveToConvergancePoint=false;;
}

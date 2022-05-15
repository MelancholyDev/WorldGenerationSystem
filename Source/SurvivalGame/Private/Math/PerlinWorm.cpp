#include "Math/PerlinWorm.h"


PerlinWorm::PerlinWorm(float*** UndergroundMapParam,float*** TempMapParam,FWormSettings WormSettingsParam, FIntVector StartPositionParam)
{
	UndergroundMap=UndergroundMapParam;
	TempMap=TempMapParam;
	WormSetings=WormSettingsParam;
	CurrentPosition = StartPositionParam;
	MoveToConvergancePoint=false;;
}

void PerlinWorm::MoveLength(int Length)
{
	 for (int i=0;i<Length;i++)
	 {
	 		Move();
	 }
}

void PerlinWorm::Move()
{
	FIntVector Direction=GetDirection();
}

FIntVector PerlinWorm::GetDirection()
{
	float Noise = UndergroundMap[CurrentPosition.X][CurrentPosition.Y][CurrentPosition.Z];
	return FIntVector();
}

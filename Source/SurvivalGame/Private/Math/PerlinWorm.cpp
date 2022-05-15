#include "Math/PerlinWorm.h"

#include <ThirdParty/CryptoPP/5.6.5/include/cryptlib.h>


PerlinWorm::PerlinWorm(float*** UndergroundMapParam, float*** TempMapParam, FWormSettings WormSettingsParam,
                       FIntVector StartPositionParam, int SizeParam, int DepthParam, int WidthParam)
{
	Width = WidthParam - 1;
	Size = SizeParam;
	Depth = DepthParam;
	UndergroundMap = UndergroundMapParam;
	TempMap = TempMapParam;
	WormSetings = WormSettingsParam;
	CurrentPosition = StartPositionParam;
	MoveToConvergancePoint = false;
}

void PerlinWorm::MoveLength(int Length)
{
	for (int i = 0; i < Length; i++)
	{
		Move();
	}
}

void PerlinWorm::Move()
{
	FIntVector Direction = GetDirection();
	FIntVector NextPosition = FIntVector(CurrentPosition.X + Direction.X, CurrentPosition.Y + Direction.Y,
	                                     CurrentPosition.Z + Direction.Z);
	if ((NextPosition.X >= 0) & (NextPosition.Y >= 0) & (NextPosition.Z >= 0) & (NextPosition.X < Size) & (NextPosition.
		Y < Size) & (NextPosition.Z < Depth))
	{
		MarkEmpty(NextPosition.X, NextPosition.Y, NextPosition.Z);
	}
	CurrentPosition = NextPosition;
}

void PerlinWorm::MarkEmpty(int X, int Y, int Z)
{
	for (int i = X - Width; i <= X + Width; i++)
		for (int j = Y - Width; j <= Y + Width; j++)
			for (int k = Z - Width; k <= Z + Width; k++)
			{
				if ((i >= 0) & (j >= 0) & (k >= 0) & (i < Size) & (j < Size) & (k < Depth))
					TempMap[i][j][k] = 0;
			}
}

FIntVector PerlinWorm::GetDirection()
{
	int X;
	int Y;
	int Z;
	float Noise;
	if ((CurrentPosition.X >= 0) & (CurrentPosition.Y >= 0) & (CurrentPosition.Z >= 0) & (CurrentPosition.X < Size) & (
		CurrentPosition.
		Y < Size) & (CurrentPosition.Z < Depth))
	{
		Noise = UndergroundMap[CurrentPosition.X][CurrentPosition.Y][CurrentPosition.Z];
	}
	else
	{
		Noise = FMath::SRand();
	}
	float Segments = 26;
	float Step = 1 / Segments;
	float Sum = 0;
	float NoiseSum = 0;
	while (Sum < Noise)
	{
		Sum += Step;
		NoiseSum++;
	}

	int NoiseSumClamped = (int)NoiseSum;
	Z = NoiseSumClamped % 3;
	int Current = NoiseSumClamped / 3;
	if (Current == 0)
	{
		Y = 0;
		X = 0;
	}
	else
	{
		Y = Current % 3;
		Current = Current / 3;
		if (Current == 0)
		{
			X = 0;
		}
		else
		{
			X = Current % 3;
		}
	}
	X = X - 1;
	Y = Y - 1;
	Z = Z - 1;
	Z += 1;
	if (Z == 2)
	{
		Z = 1;
		Y += 1;
		if (Y == 2)
		{
			Y = 1;
			X += 1;
			if (X == 2)
			{
				X = 1;
			}
		}
	}
	FIntVector Result(X, Y, Z);
	return Result;
}

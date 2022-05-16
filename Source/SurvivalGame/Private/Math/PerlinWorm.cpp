#include "Math/PerlinWorm.h"

PerlinWorm::PerlinWorm(float*** UndergroundMapParam, float*** FirstNoiseParam, float*** SecondNoiseParam,
                       FWormSettings WormSettingsParam,
                       FIntVector StartPositionParam, int SizeParam, int DepthParam)
{
	Width = WormSettingsParam.WormWidth - 1;
	Size = SizeParam;
	Depth = DepthParam;
	UndergroundMap = UndergroundMapParam;
	FirstNoise = FirstNoiseParam;
	SecondNoise = SecondNoiseParam;
	WormSetings = WormSettingsParam;
	CurrentPosition = StartPositionParam;
	MoveToConvergancePoint = false;
}

void PerlinWorm::MoveLength(int Length)
{
	FMath::SRandInit(CurrentPosition.X + CurrentPosition.Y + CurrentPosition.Z);
	//int CurrentDirectionX = FMath::RandRange(-1, 1);
	const int CurrentDirectionX = 1;
	//int CurrentDirectionY = FMath::RandRange(-1, 1);
	const int CurrentDirectionY = 0;
	//int CurrentDirectionZ = FMath::RandRange(-1,1);
	const int CurrentDirectionZ = 0;
	CurrentDirection = FIntVector(CurrentDirectionX, CurrentDirectionY, CurrentDirectionZ);
	UniqueBlocks=0;
	for (int i = 0; i < Length; i++)
	{
		Move();
	}
}

void PerlinWorm::Move()
{
	const FIntVector Direction = GetDirection();
	const FIntVector NextPosition = FIntVector(CurrentPosition.X + Direction.X, CurrentPosition.Y + Direction.Y,
	                                           CurrentPosition.Z + Direction.Z);
	if ((NextPosition.X >= 0) & (NextPosition.Y >= 0) & (NextPosition.Z >= 0) & (NextPosition.X < Size) & (NextPosition.
		Y < Size) & (NextPosition.Z < Depth))
	{
		if(UndergroundMap[NextPosition.X][NextPosition.Y][NextPosition.Z]!=0)
			UniqueBlocks++;
		MarkEmpty(NextPosition.X, NextPosition.Y, NextPosition.Z);
	}
	CurrentPosition = NextPosition;
	CurrentDirection = Direction;
}

void PerlinWorm::MarkEmpty(int X, int Y, int Z) const
{
	for (int i = X - Width; i <= X + Width; i++)
		for (int j = Y - Width; j <= Y + Width; j++)
			for (int k = Z - Width; k <= Z + Width; k++)
			{
				if ((i >= 0) & (j >= 0) & (k >= 0) & (i < Size) & (j < Size) & (k < Depth))
					UndergroundMap[i][j][k] = 0;
			}
}

FIntVector PerlinWorm::GetDirection()
{
	float FirstNoiseCurrent;
	float SecondNoiseCurrent;
	if ((CurrentPosition.X >= 0) & (CurrentPosition.Y >= 0) & (CurrentPosition.Z >= 0) & (CurrentPosition.X < Size) & (
		CurrentPosition.
		Y < Size) & (CurrentPosition.Z < Depth))
	{
		FirstNoiseCurrent = FirstNoise[CurrentPosition.X][CurrentPosition.Y][CurrentPosition.Z];
		SecondNoiseCurrent = SecondNoise[CurrentPosition.X][CurrentPosition.Y][CurrentPosition.Z];
	}
	else
	{
		return FIntVector(0, 0, 0);
	}
	float Angle;
	if ((FirstNoiseCurrent >= 0) & (FirstNoiseCurrent < 0.2))
	{
		Angle = -90;
	}
	else if ((FirstNoiseCurrent >= 0.2) & (FirstNoiseCurrent < 0.4))
	{
		Angle = -45;
	}
	else if ((FirstNoiseCurrent >= 0.4) & (FirstNoiseCurrent < 0.6))
	{
		Angle = 0;
	}
	else if ((FirstNoiseCurrent >= 0.6) & (FirstNoiseCurrent < 0.8))
	{
		Angle = 45;
	}
	else
	{
		Angle = 90;
	}

	const float AngleInRadians = Angle * PI / 180;

	const float ComputedX = FMath::Cos(AngleInRadians) * CurrentDirection.X - FMath::Sin(AngleInRadians) * CurrentDirection.Y;
	const float ComputedY = FMath::Sin(AngleInRadians) * CurrentDirection.X + FMath::Cos(AngleInRadians) * CurrentDirection.Y;

	FVector NextDirection = FVector(ComputedX, ComputedY, 0);
	if ((Angle == -45) | (Angle == 45))
	{
		if (NextDirection.Size() > 1)
		{
			NextDirection.Normalize();
		}
		else
		{
			NextDirection.X = NextDirection.X / FMath::Sqrt(2) * 2;
			NextDirection.Y = NextDirection.Y / FMath::Sqrt(2) * 2;
		}
	}
	FIntVector NextDirectionInt=FIntVector(FMath::Floor(NextDirection.X+0.1),FMath::Floor(NextDirection.Y+0.1),0);
	int Z;
	if((SecondNoiseCurrent>=0) & (SecondNoiseCurrent<0.33))
	{
		Z=-1;
	}else if((SecondNoiseCurrent>=0.33) & (SecondNoiseCurrent<0.66))
	{
		Z=0;
	}else
	{
		Z=1;
	}
	NextDirectionInt.Z=Z;
	return NextDirectionInt;
}

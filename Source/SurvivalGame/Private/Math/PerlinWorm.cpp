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
	int CurrentDirectionX = 1;
	//int CurrentDirectionY = FMath::RandRange(-1, 1);
	int CurrentDirectionY = 0;
	//int CurrentDirectionZ = FMath::RandRange(-1,1);
	int CurrentDirectionZ = 0;
	CurrentDirection = FIntVector(CurrentDirectionX, CurrentDirectionY, CurrentDirectionZ);
	UniqueBlocks=0;
	for (int i = 0; i < Length; i++)
	{
		Move();
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
					               FString::Printf(TEXT(" %d  UNIQUE BLOCKS!"),UniqueBlocks));
}

void PerlinWorm::Move()
{
	FIntVector Direction = GetDirection();
	FIntVector NextPosition = FIntVector(CurrentPosition.X + Direction.X, CurrentPosition.Y + Direction.Y,
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

void PerlinWorm::MarkEmpty(int X, int Y, int Z)
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

	float AngleInRadians = Angle * PI / 180;

	float ComputedX = FMath::Cos(AngleInRadians) * CurrentDirection.X - FMath::Sin(AngleInRadians) * CurrentDirection.Y;
	float ComputedY = FMath::Sin(AngleInRadians) * CurrentDirection.X + FMath::Cos(AngleInRadians) * CurrentDirection.Y;

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
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	                  //               FString::Printf(TEXT(" %d %d"),NextDirectionInt.X, NextDirectionInt.Y));
	// if (((ComputedX != -1) & (ComputedX != 0) & (ComputedX != 1)) | ((ComputedY != -1) & (ComputedY != 0) & (ComputedY != 1)))
	// 		if((NextDirection.X==0) | (NextDirection.Y==0))
	// 		{
	// 			NextDirection.Normalize();
	// 		}else
	// 		{
	// 			int A;
	// 			int B;
	// 			//if((ComputedX != -1) & (ComputedX != 0) & (ComputedX != 1))
	// 			if(NextDirection.X > 1.0)
	// 				A=0;
	// 			else
	// 				A=1;
	//
	// 			if(((ComputedY != -1) & (ComputedY != 0) & (ComputedY != 1)))
	// 				B=0;
	// 			else
	// 				B=1;
	// 			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	// 										FString::Printf(TEXT("%f %f %d %d"), NextDirection.X, NextDirection.Y,A,B));
	// 			 NextDirection.X=NextDirection.X*FMath::Sqrt(2)/2;
	// 		   	 NextDirection.Y=NextDirection.Y*FMath::Sqrt(2)/2;
	// 			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	// 										FString::Printf(TEXT("%f %f"), NextDirection.X, NextDirection.Y));
	// 		}
	// if (NextDirection.Size() > 1)
	// {
	// 	NextDirection.Normalize();
	// }else
	// {
	// 	NextDirection.X=NextDirection.X*2/FMath::Square(2);
	// 	NextDirection.Y=NextDirection.Y*2/FMath::Square(2);
	// }
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	//                                  FString::Printf(TEXT("%f %f %d %d"), NextDirection.X, NextDirection.Y,
	//                                                  CurrentVectorX, CurrentVectorY));
	return FIntVector(0, 0, 0);
	// float Segments = 26;
	// float Step = 1 / Segments;
	// float Sum = 0;
	// float NoiseSum = 0;
	// while (Sum < FirstNoiseCurrent)
	// {
	// 	Sum += Step;
	// 	NoiseSum++;
	// }
	//
	// int NoiseSumClamped = (int)NoiseSum;
	// Z = NoiseSumClamped % 3;
	// int Current = NoiseSumClamped / 3;
	// if (Current == 0)
	// {
	// 	Y = 0;
	// 	X = 0;
	// }
	// else
	// {
	// 	Y = Current % 3;
	// 	Current = Current / 3;
	// 	if (Current == 0)
	// 	{
	// 		X = 0;
	// 	}
	// 	else
	// 	{
	// 		X = Current % 3;
	// 	}
	// }
	// X = X - 1;
	// Y = Y - 1;
	// Z = Z - 1;
	// Z += 1;
	// if (Z == 2)
	// {
	// 	Z = 1;
	// 	Y += 1;
	// 	if (Y == 2)
	// 	{
	// 		Y = 1;
	// 		X += 1;
	// 		if (X == 2)
	// 		{
	// 			X = 1;
	// 		}
	// 	}
	// }
	// FIntVector Result(X, Y, Z);
	// return Result;
}

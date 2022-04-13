#include "GenerationPlayerController.h"

#include <string>

#include "Kismet/GameplayStatics.h"
#include "Math/IntVector.h"

FIntVector AGenerationPlayerController::GetPlayerChunkCoordinates()
{
	FVector location = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	location = location / ChunkLength;
	return FIntVector(round(location.X), round(location.Y), 0);
}

AVoxelChank* AGenerationPlayerController::SpawnChunk(float X, float Y, float Z)
{
	FString str = FString::Printf(TEXT("%f , %f , %f"), X, Y, Z);
	const FVector Location = {X, Y, Z};
	const FTransform Transform = FTransform(Location);
	AActor* NewActor = GetWorld()->SpawnActorDeferred<AVoxelChank>(AVoxelChank::StaticClass(), Transform);
	AVoxelChank* Chunk = Cast<AVoxelChank>(NewActor);
	Chunk->InitializeParameters(NoiseDensity, VoxelSize, NoiseScale, ChunkSize, Depth,
	                            NoiseDensity3D, Threshold3D,WaterLevel);
	UGameplayStatics::FinishSpawningActor(NewActor, Transform);
	return Chunk;
}


void AGenerationPlayerController::DeleteLine(int Index)
{
	TArray<AVoxelChank*> Line = (*Map)[Index].Voxels;
	while (Line.Num() > 0)
	{
		int a = Line[0]->Children.Num();
		std::string s = std::to_string(a);
		AVoxelChank* DeleteChunk = Line[0];
		Line.RemoveAt(0);
		DeleteChunk->Destroy();
	}
	(*Map).RemoveAt(Index);
}

void AGenerationPlayerController::AddLine(bool IsForward)
{
	auto CurrentCoordinates = GetPlayerChunkCoordinates();
	int Shift = IsForward ? RenderRange : -RenderRange;
	FVoxelLine Line = FVoxelLine();
	for (int i = RenderRange * -1; i <= RenderRange; i++)
	{
		AVoxelChank* CreatedChunk = SpawnChunk((CurrentCoordinates.X + Shift) * ChunkLength,
		                                       (CurrentCoordinates.Y + i) * ChunkLength, 0);
		Line.Voxels.Add(CreatedChunk);
	}
	if (IsForward)
	{
		Map->Insert(Line, RenderRange * 2);
	}
	else
	{
		Map->Insert(Line, 0);
	}
}

void AGenerationPlayerController::AddColumn(bool isLeft)
{
	auto CurrentCoordinates = GetPlayerChunkCoordinates();
	int Shift = isLeft ? -RenderRange : RenderRange;
	for (int i = 0; i <= RenderRange * 2; i++)
	{
		AVoxelChank* CreatedChunk = SpawnChunk((CurrentCoordinates.X + i - RenderRange) * ChunkLength,
		                                       (CurrentCoordinates.Y + Shift) * ChunkLength, 0);
		FVoxelLine* Line = &(*Map)[i];
		if (isLeft)
		{
			Line->Voxels.Insert(CreatedChunk, 0);
		}
		else
		{
			Line->Voxels.Insert(CreatedChunk, RenderRange * 2);
		}
	}
}

void AGenerationPlayerController::AppendColumn(int Index, bool isLeft)
{
	int Start;
	int Last;
	int Increment;
	int XShift;
	if (Index == 0)
	{
		Start = 1;
		Last = RenderRange * 2;
		Increment = 1;
		XShift = 0;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Z"));
		Start = RenderRange * 2 - 1;
		Last = 0;
		Increment = -1;
		XShift = 0;
	}
	auto CurrentCoordinates = GetPlayerChunkCoordinates();
	int Shift = isLeft ? -RenderRange : RenderRange;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("B"));
	for (int i = Start; Index > 0 ? i >= Last : i <= Last; i += Increment)
	{
		FString a = FString::Printf(TEXT("TREXT:%d"), i - RenderRange + XShift);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("AAA") + a);
		FVoxelLine* Line = &(*Map)[i];
		AVoxelChank* chunk = SpawnChunk((CurrentCoordinates.X + i - RenderRange + XShift) * ChunkLength,
		                                (CurrentCoordinates.Y + Shift) * ChunkLength, 0);
		a = FString::Printf(TEXT("%f %f %f"), chunk->GetActorLocation().X, chunk->GetActorLocation().Y,
		                    chunk->GetActorLocation().Z);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, a);
		if (isLeft)
		{
			Line->Voxels.Insert(chunk, 0);
		}
		else
		{
			Line->Voxels.Add(chunk);
		}
	}
}

void AGenerationPlayerController::DeleteColumn(int Index)
{
	for (int i = 0; i < (*Map).Num(); i++)
	{
		FVoxelLine* Line = &(*Map)[i];
		AVoxelChank* DeleteChunk = Line->Voxels[Index];
		Line->Voxels.RemoveAt(Index);
		DeleteChunk->Destroy();
	}
}

void AGenerationPlayerController::GetFullSize()
{
	for (int i = 0; i <= RenderRange * 2; i++)
	{
		FVoxelLine* Line = &(*Map)[i];
		FString a = FString::Printf(TEXT("%d %d"), Line->Voxels.Num(), i);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, a);
		for (int j = 0; j < Line->Voxels.Num(); j++)
		{
			AVoxelChank* ch = Line->Voxels[j];
			a = FString::Printf(TEXT("%f %f %f"), ch->GetActorLocation().X, ch->GetActorLocation().Y,
			                    ch->GetActorLocation().Z);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, a);
		}
	}
}

void AGenerationPlayerController::XShift(int X)
{
	if (X > 0)
	{
		DeleteLine(0);
		AddLine(true);
	}
	else
	{
		DeleteLine(RenderRange * 2);
		AddLine(false);
	}
}

void AGenerationPlayerController::YShift(int Y)
{
	if (Y < 0)
	{
		DeleteColumn(RenderRange * 2);
		AddColumn(true);
	}
	else
	{
		DeleteColumn(0);
		AddColumn(false);
	}
}

void AGenerationPlayerController::Diagonal(int X, int Y)
{
	FString a = FString::Printf(TEXT("%d %d"), X, Y);
	if (X > 0)
	{
		if (Y > 0)
		{
			DeleteLine(0);
			DeleteColumn(0);
			AddLine(true);
			AppendColumn(RenderRange * 2, false);
		}
		else
		{
			DeleteLine(0);
			DeleteColumn(RenderRange * 2);
			AddLine(true);
			AppendColumn(RenderRange * 2, true);
		}
	}
	else
	{
		if (Y > 0)
		{
			DeleteLine(RenderRange * 2);
			DeleteColumn(0);
			AddLine(false);
			AppendColumn(0, false);
		}
		else
		{
			DeleteLine(RenderRange * 2);
			DeleteColumn(RenderRange * 2);
			AddLine(false);
			AppendColumn(0, true);
		}
	}
}

void AGenerationPlayerController::BeginPlay()
{
	ChunkLength = ChunkSize*VoxelSize*2+VoxelSize;
	OldCoordinates = GetPlayerChunkCoordinates();
	for (int i = RenderRange * -1; i <= RenderRange; i++)
	{
		for (int j = RenderRange * -1; j <= RenderRange; j++)
		{
			FIntVector Vector = GetPlayerChunkCoordinates();
			AVoxelChank* chunk = SpawnChunk((Vector.X + i) * ChunkLength, (Vector.Y + j) * ChunkLength, 0);
			(*Map)[i + RenderRange].Voxels.Add(chunk);
		}
	}
}

void AGenerationPlayerController::Tick(float DeltaSeconds)
{
	std::string s = std::to_string(RenderRange);
	FString str = s.c_str();
	auto CurrentCoordinates = GetPlayerChunkCoordinates();
	int X_Shift = CurrentCoordinates.X - OldCoordinates.X;
	int Y_Shift = CurrentCoordinates.Y - OldCoordinates.Y;
	if ((X_Shift != 0) & (Y_Shift != 0))
	{
		Diagonal(X_Shift, Y_Shift);
	}
	else if (X_Shift != 0)
	{
		XShift(X_Shift);
	}
	else if (Y_Shift != 0)
	{
		YShift(Y_Shift);
	}
	OldCoordinates = CurrentCoordinates;
}

void AGenerationPlayerController::OnConstruction(const FTransform& Transform)
{
	ChunkSpawnParameters = new FActorSpawnParameters();
	Map = new TArray<FVoxelLine>();
	for (int i = RenderRange * -1; i <= RenderRange; i++)
	{
		Map->Add(FVoxelLine());
	}
}

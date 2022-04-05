#include "GenerationPlayerController.h"

#include <string>

#include "Math/IntVector.h"

FIntVector AGenerationPlayerController::GetPlayerChunkCoordinates()
{
	FVector location = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	location = location / ChunkSize;
	return FIntVector(round(location.X), round(location.Y), 0);
}

AVoxelChank* AGenerationPlayerController::SpawnChunk(float X, float Y, float Z) const
{
	const FVector Location = {X, Y, Z};
	const FRotator Rotation = FRotator(0, 0, 0);
	AActor* NewActor = GetWorld()->SpawnActor(ToSpawn, &Location, &Rotation);
	AVoxelChank* Chunk = Cast<AVoxelChank>(NewActor);
	return Chunk;
}

void AGenerationPlayerController::DeleteColumn(int Index)
{
}

void AGenerationPlayerController::DeleteLine(int Index)
{
	TArray<AVoxelChank*> Line = (*Map)[Index].Voxels;
	while (Line.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("1!"));
		AVoxelChank* DeleteChunk = Line[0];
		Line.RemoveAt(0);
		DeleteChunk->Destroy();
	}
}

void AGenerationPlayerController::AddLine(int Index)
{
	auto CurrentCoordinates = GetPlayerChunkCoordinates();
	FVoxelLine Line = FVoxelLine();
	for (int i = RenderRange * -1; i <= RenderRange; i++)
	{
		AVoxelChank* CreatedChunk = SpawnChunk(CurrentCoordinates.X * ChunkSize, CurrentCoordinates.Y * ChunkSize, 0);
		Line.Voxels.Add(CreatedChunk);
	}
}

void AGenerationPlayerController::AddColumn(int Index)
{
}

void AGenerationPlayerController::XShift(int X)
{
	if (X > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("1!"));
		DeleteLine(0);
		AddLine(RenderRange * 2);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("2!"));
		DeleteLine(RenderRange * 2);
		AddLine(0);
	}
}

void AGenerationPlayerController::YShift(int Y)
{
}

void AGenerationPlayerController::Diagonal(int X, int Y)
{
}

void AGenerationPlayerController::BeginPlay()
{
	OldCoordinates = GetPlayerChunkCoordinates();
	for (int i = RenderRange * -1; i <= RenderRange; i++)
	{
		for (int j = RenderRange * -1; j <= RenderRange; j++)
		{
			FIntVector Vector = GetPlayerChunkCoordinates();
			AVoxelChank* chunk = SpawnChunk((Vector.X + i) * ChunkSize, (Vector.Y + j) * ChunkSize, 0);
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
	Map = new TArray<FVoxelLine>();
	for (int i = RenderRange * -1; i <= RenderRange; i++)
	{
		Map->Add(FVoxelLine());
	}
}

FAsyncSpawnChunkTask::FAsyncSpawnChunkTask(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;

}


void FAsyncSpawnChunkTask::DoWork()
{
	// const FVector Location = {x, y, z};
	// const FRotator Rotation = FRotator(0, 0, 0);
	//
	// AActor* NewActor = GetWorld()->SpawnActor(ToSpawn, &Location, &Rotation);
	// AVoxelChank* Chunk = Cast<AVoxelChank>(NewActor);
}

#include "..\Public\WorldGenerationSubsystem.h"
#include <string>
#include "Kismet/GameplayStatics.h"
#include "Math/IntVector.h"

FIntVector UWorldGenerationSubsystem::GetPlayerChunkCoordinates()
{
	FVector location = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	location = location / ChunkLength;
	return FIntVector(round(location.X), round(location.Y), 0);
}


AVoxelChank* UWorldGenerationSubsystem::SpawnChunk(float X, float Y, float Z)
{
	const FVector Location = {X, Y, Z};
	const FTransform Transform = FTransform(Location);
	AActor* NewActor = GetWorld()->SpawnActorDeferred<AVoxelChank>(AVoxelChank::StaticClass(), Transform);
	AVoxelChank* Chunk = Cast<AVoxelChank>(NewActor);
	Chunk->InitializeParameters(VoxelGenerationData);
	UGameplayStatics::FinishSpawningActor(NewActor, Transform);
	return Chunk;
}


void UWorldGenerationSubsystem::DeleteLine(int Index)
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

void UWorldGenerationSubsystem::AddLine(bool IsForward)
{
	auto CurrentCoordinates = GetPlayerChunkCoordinates();
	int Shift = IsForward ? GenerationSettings->RenderRange : -GenerationSettings->RenderRange;
	FVoxelLine Line = FVoxelLine();
	for (int i = GenerationSettings->RenderRange * -1; i <= GenerationSettings->RenderRange; i++)
	{
		AVoxelChank* CreatedChunk = SpawnChunk((CurrentCoordinates.X + Shift) * ChunkLength,
		                                       (CurrentCoordinates.Y + i) * ChunkLength, 0);
		Line.Voxels.Add(CreatedChunk);
	}
	if (IsForward)
	{
		Map->Insert(Line, GenerationSettings->RenderRange * 2);
	}
	else
	{
		Map->Insert(Line, 0);
	}
}

void UWorldGenerationSubsystem::AddColumn(bool isLeft)
{
	auto CurrentCoordinates = GetPlayerChunkCoordinates();
	int Shift = isLeft ? -GenerationSettings->RenderRange : GenerationSettings->RenderRange;
	for (int i = 0; i <= GenerationSettings->RenderRange * 2; i++)
	{
		AVoxelChank* CreatedChunk = SpawnChunk(
			(CurrentCoordinates.X + i - GenerationSettings->RenderRange) * ChunkLength,
			(CurrentCoordinates.Y + Shift) * ChunkLength, 0);
		FVoxelLine* Line = &(*Map)[i];
		if (isLeft)
		{
			Line->Voxels.Insert(CreatedChunk, 0);
		}
		else
		{
			Line->Voxels.Insert(CreatedChunk, GenerationSettings->RenderRange * 2);
		}
	}
}

void UWorldGenerationSubsystem::AppendColumn(int Index, bool isLeft)
{
	int Start;
	int Last;
	int Increment;
	int XShift;
	if (Index == 0)
	{
		Start = 1;
		Last = GenerationSettings->RenderRange * 2;
		Increment = 1;
		XShift = 0;
	}
	else
	{
		Start = GenerationSettings->RenderRange * 2 - 1;
		Last = 0;
		Increment = -1;
		XShift = 0;
	}
	auto CurrentCoordinates = GetPlayerChunkCoordinates();
	int Shift = isLeft ? -GenerationSettings->RenderRange : GenerationSettings->RenderRange;
	for (int i = Start; Index > 0 ? i >= Last : i <= Last; i += Increment)
	{
		FVoxelLine* Line = &(*Map)[i];
		AVoxelChank* chunk = SpawnChunk(
			(CurrentCoordinates.X + i - GenerationSettings->RenderRange + XShift) * ChunkLength,
			(CurrentCoordinates.Y + Shift) * ChunkLength, 0);
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

void UWorldGenerationSubsystem::DeleteColumn(int Index)
{
	for (int i = 0; i < (*Map).Num(); i++)
	{
		FVoxelLine* Line = &(*Map)[i];
		AVoxelChank* DeleteChunk = Line->Voxels[Index];
		Line->Voxels.RemoveAt(Index);
		DeleteChunk->Destroy();
	}
}

void UWorldGenerationSubsystem::InitializeParameters()
{
	if (GenerationSettings->GenerationType == PERLIN_NOISE)
	{
		float Multiplier = GenerationSettings->PerlinNoiseParameters.MapSizeMultiplier;
		MapSize = (GenerationSettings->ChunkSize * 2 + 1) * Multiplier;
	}
	else
	{
		MapSize = FMath::Pow(2, GenerationSettings->DiamondSquareParameters.MapMultiplier) + 1;
	}
	ChunkRenderLines = new FActorSpawnParameters();
	Map = new TArray<FVoxelLine>();
	for (int i = GenerationSettings->RenderRange * -1; i <= GenerationSettings->RenderRange; i++)
	{
		Map->Add(FVoxelLine());
	}
	GeneratorInstance = new Generator(GenerationSettings);
}


void UWorldGenerationSubsystem::GenerateWorld(UWorldGenerationSettings* WorldGenerationSystem)
{
	GenerationSettings = WorldGenerationSystem;
	InitializeParameters();
	GenerateMaps();

	ChunkLength = GenerationSettings->ChunkSize * GenerationSettings->VoxelSize * 2 + GenerationSettings->VoxelSize;
	OldCoordinates = GetPlayerChunkCoordinates();
	for (int i = GenerationSettings->RenderRange * -1; i <= GenerationSettings->RenderRange; i++)
	{
		for (int j = GenerationSettings->RenderRange * -1; j <= GenerationSettings->RenderRange; j++)
		{
			FIntVector Vector = GetPlayerChunkCoordinates();
			int XCoord = (Vector.X + i) * ChunkLength;
			int YCoord = (Vector.Y + j) * ChunkLength;
			AVoxelChank* chunk = SpawnChunk(XCoord, YCoord, 0);
			(*Map)[i + GenerationSettings->RenderRange].Voxels.Add(chunk);
		}
	}
}

void UWorldGenerationSubsystem::GetFullSize()
{
	for (int i = 0; i <= GenerationSettings->RenderRange * 2; i++)
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


void UWorldGenerationSubsystem::XShift(int X)
{
	if (X > 0)
	{
		DeleteLine(0);
		AddLine(true);
	}
	else
	{
		DeleteLine(GenerationSettings->RenderRange * 2);
		AddLine(false);
	}
}

void UWorldGenerationSubsystem::YShift(int Y)
{
	if (Y < 0)
	{
		DeleteColumn(GenerationSettings->RenderRange * 2);
		AddColumn(true);
	}
	else
	{
		DeleteColumn(0);
		AddColumn(false);
	}
}

void UWorldGenerationSubsystem::Diagonal(int X, int Y)
{
	FString a = FString::Printf(TEXT("%d %d"), X, Y);
	if (X > 0)
	{
		if (Y > 0)
		{
			DeleteLine(0);
			DeleteColumn(0);
			AddLine(true);
			AppendColumn(GenerationSettings->RenderRange * 2, false);
		}
		else
		{
			DeleteLine(0);
			DeleteColumn(GenerationSettings->RenderRange * 2);
			AddLine(true);
			AppendColumn(GenerationSettings->RenderRange * 2, true);
		}
	}
	else
	{
		if (Y > 0)
		{
			DeleteLine(GenerationSettings->RenderRange * 2);
			DeleteColumn(0);
			AddLine(false);
			AppendColumn(0, false);
		}
		else
		{
			DeleteLine(GenerationSettings->RenderRange * 2);
			DeleteColumn(GenerationSettings->RenderRange * 2);
			AddLine(false);
			AppendColumn(0, true);
		}
	}
}

//
// void UWorldGenerationSubsystem::Tick(float DeltaSeconds)
// {
// 	auto CurrentCoordinates = GetPlayerChunkCoordinates();
// 	int X_Shift = CurrentCoordinates.X - OldCoordinates.X;
// 	int Y_Shift = CurrentCoordinates.Y - OldCoordinates.Y;
// 	if ((X_Shift != 0) & (Y_Shift != 0))
// 	{
// 		Diagonal(X_Shift, Y_Shift);
// 	}
// 	else if (X_Shift != 0)
// 	{
// 		XShift(X_Shift);
// 	}
// 	else if (Y_Shift != 0)
// 	{
// 		YShift(Y_Shift);
// 	}
// 	OldCoordinates = CurrentCoordinates;
// }


void UWorldGenerationSubsystem::GenerateMaps()
{
	HeightMap = new float*[MapSize];
	WaterMap = new float*[MapSize];
	UndergroundMap = new float**[MapSize];
	BiomMap = new EBiomType*[MapSize];

	for (int i = 0; i < MapSize; i++)
	{
		HeightMap[i] = new float[MapSize];
		WaterMap[i] = new float[MapSize];
		UndergroundMap[i] = new float*[MapSize];
		BiomMap[i] = new EBiomType[MapSize];
	}
	for (int i = 0; i < MapSize; i++)
	{
		for (int j = 0; j < MapSize; j++)
		{
			UndergroundMap[i][j] = new float[GenerationSettings->UndergroundParameters.CaveStart - GenerationSettings->
				UndergroundParameters.Depth + 1];
		}
	}
	GenerateBiomMap();
	GenerateHeightMap();
	GenerateCaveMap();
	VoxelGenerationData = FVoxelGenerationData(GenerationSettings, MapSize, HeightMap, BiomMap, UndergroundMap);
}

void UWorldGenerationSubsystem::GenerateHeightMap() const
{
	GeneratorInstance->GenerateHeightMap(HeightMap, BiomMap);
}

void UWorldGenerationSubsystem::GenerateBiomMap() const
{
	GeneratorInstance->GenerateBiomMaps(BiomMap);
}

void UWorldGenerationSubsystem::GenerateCaveMap() const
{
	GeneratorInstance->GenerateCaveMap(UndergroundMap);
}

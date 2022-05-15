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
	const FVector Location = {X, Y, Z};
	const FTransform Transform = FTransform(Location);
	AActor* NewActor = GetWorld()->SpawnActorDeferred<AVoxelChank>(AVoxelChank::StaticClass(), Transform);
	AVoxelChank* Chunk = Cast<AVoxelChank>(NewActor);
	FVoxelGenerationData Data;
	Data.Initialize(VoxelGenerationData.IsAddDepth, GenerationParameters.VoxelSize, GenerationParameters.PerlinNoiseParameters.NoiseScale,
	                GenerationParameters.ChunkSize, VoxelGenerationData.Depth,VoxelGenerationData.CaveStart, VoxelGenerationData.NoiseDensity3D,
	                VoxelGenerationData.Threshold3D, MapSize, HeightMap, HeatMap,UndergroundMap);
	Chunk->InitializeParameters(Data,WaterLevel);
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
	int Shift = IsForward ? GenerationParameters.RenderRange : -GenerationParameters.RenderRange;
	FVoxelLine Line = FVoxelLine();
	for (int i = GenerationParameters.RenderRange * -1; i <= GenerationParameters.RenderRange; i++)
	{
		AVoxelChank* CreatedChunk = SpawnChunk((CurrentCoordinates.X + Shift) * ChunkLength,
		                                       (CurrentCoordinates.Y + i) * ChunkLength, 0);
		Line.Voxels.Add(CreatedChunk);
	}
	if (IsForward)
	{
		Map->Insert(Line, GenerationParameters.RenderRange * 2);
	}
	else
	{
		Map->Insert(Line, 0);
	}
}

void AGenerationPlayerController::AddColumn(bool isLeft)
{
	auto CurrentCoordinates = GetPlayerChunkCoordinates();
	int Shift = isLeft ? -GenerationParameters.RenderRange : GenerationParameters.RenderRange;
	for (int i = 0; i <= GenerationParameters.RenderRange * 2; i++)
	{
		AVoxelChank* CreatedChunk = SpawnChunk((CurrentCoordinates.X + i - GenerationParameters.RenderRange) * ChunkLength,
		                                       (CurrentCoordinates.Y + Shift) * ChunkLength, 0);
		FVoxelLine* Line = &(*Map)[i];
		if (isLeft)
		{
			Line->Voxels.Insert(CreatedChunk, 0);
		}
		else
		{
			Line->Voxels.Insert(CreatedChunk, GenerationParameters.RenderRange * 2);
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
		Last = GenerationParameters.RenderRange * 2;
		Increment = 1;
		XShift = 0;
	}
	else
	{
		Start = GenerationParameters.RenderRange * 2 - 1;
		Last = 0;
		Increment = -1;
		XShift = 0;
	}
	auto CurrentCoordinates = GetPlayerChunkCoordinates();
	int Shift = isLeft ? -GenerationParameters.RenderRange : GenerationParameters.RenderRange;
	for (int i = Start; Index > 0 ? i >= Last : i <= Last; i += Increment)
	{
		FVoxelLine* Line = &(*Map)[i];
		AVoxelChank* chunk = SpawnChunk(
			(CurrentCoordinates.X + i - GenerationParameters.RenderRange + XShift) * ChunkLength,
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

void AGenerationPlayerController::InitializeParameters()
{
	if (GenerationParameters.GenerationType == PERLIN_NOISE)
	{
		float Multiplier = GenerationParameters.PerlinNoiseParameters.Multiplier;
		MapSize = (GenerationParameters.ChunkSize * 2 + 1) * Multiplier;
	}
	else
	{
		MapSize = FMath::Pow(2, GenerationParameters.DiamondSquareParameters.MapMultiplier) + 1;
	}
	ChunkRenderLines = new FActorSpawnParameters();
	Map = new TArray<FVoxelLine>();
	for (int i = GenerationParameters.RenderRange * -1; i <= GenerationParameters.RenderRange; i++)
	{
		Map->Add(FVoxelLine());
	}
	GeneratorInstance=new Generator(GenerationParameters,VoxelGenerationData,BiomDataSet);
}


void AGenerationPlayerController::GetFullSize()
{
	for (int i = 0; i <= GenerationParameters.RenderRange * 2; i++)
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
		DeleteLine(GenerationParameters.RenderRange * 2);
		AddLine(false);
	}
}

void AGenerationPlayerController::YShift(int Y)
{
	if (Y < 0)
	{
		DeleteColumn(GenerationParameters.RenderRange * 2);
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
			AppendColumn(GenerationParameters.RenderRange * 2, false);
		}
		else
		{
			DeleteLine(0);
			DeleteColumn(GenerationParameters.RenderRange * 2);
			AddLine(true);
			AppendColumn(GenerationParameters.RenderRange * 2, true);
		}
	}
	else
	{
		if (Y > 0)
		{
			DeleteLine(GenerationParameters.RenderRange * 2);
			DeleteColumn(0);
			AddLine(false);
			AppendColumn(0, false);
		}
		else
		{
			DeleteLine(GenerationParameters.RenderRange * 2);
			DeleteColumn(GenerationParameters.RenderRange * 2);
			AddLine(false);
			AppendColumn(0, true);
		}
	}
}

void AGenerationPlayerController::BeginPlay()
{
	InitializeParameters();
	GenerateMaps();
	
	ChunkLength = GenerationParameters.ChunkSize * GenerationParameters.VoxelSize * 2 + GenerationParameters.VoxelSize;
	OldCoordinates = GetPlayerChunkCoordinates();
	for (int i = GenerationParameters.RenderRange * -1; i <= GenerationParameters.RenderRange; i++)
	{
		for (int j = GenerationParameters.RenderRange * -1; j <= GenerationParameters.RenderRange; j++)
		{
			FIntVector Vector = GetPlayerChunkCoordinates();
			int XCoord = (Vector.X + i) * ChunkLength;
			int YCoord = (Vector.Y + j) * ChunkLength;
			AVoxelChank* chunk = SpawnChunk(XCoord, YCoord, 0);
			(*Map)[i + GenerationParameters.RenderRange].Voxels.Add(chunk);
		}
	}
}

void AGenerationPlayerController::Tick(float DeltaSeconds)
{
	if(UpdateMap)
	{
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
}

void AGenerationPlayerController::PrintFullMap()
{
	for(int i=0;i<MapSize;i++)
	{
		FString str="";
		for(int j=0;j<MapSize;j++)
		{
			str+=FString::Printf(TEXT("%f"),HeightMap[i][j])+" ";
		}
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, str);
	}
}


void AGenerationPlayerController::GenerateMaps()
{
	HeightMap = new float*[MapSize];
	HeatMap = new float*[MapSize];
	WaterMap = new float*[MapSize];
	MoistureMap = new float*[MapSize];
	UndergroundMap = new float**[MapSize];

	for (int i =0; i < MapSize; i++)
	{
		HeatMap[i] = new float[MapSize];
		HeightMap[i] = new float[MapSize];
		WaterMap[i] = new float[MapSize];
		MoistureMap[i] = new float[MapSize];
		UndergroundMap[i] = new float*[MapSize];
	}
	for (int i =0; i < MapSize; i++)
	{
		for (int j =0; j < MapSize; j++)
		{
			UndergroundMap[i][j] = new float[VoxelGenerationData.CaveStart - VoxelGenerationData.Depth+1];
		}
	}
	GenerateHeatMap();
	GenerateHeightMap();
	GenerateCaveMap();
}

void AGenerationPlayerController::GenerateHeightMap()
{
	GeneratorInstance->GenerateHeightMap(HeightMap,HeatMap,MoistureMap);
}

void AGenerationPlayerController::GenerateHeatMap()
{
	GeneratorInstance->GenerateBiomMaps(HeatMap,MoistureMap);
}

void AGenerationPlayerController::GenerateCaveMap()
{
	GeneratorInstance->GenerateCaveMap(UndergroundMap);
}



#include "GenerationPlayerController.h"

#include <string>

#include "BezierComputations.h"
#include "Kismet/GameplayStatics.h"
#include "Math/IntVector.h"
#include "Enums.h"

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
	//Chunk->InitializeParameters(VoxelSize, NoiseScale, ChunkSize, Depth, MapSize, MapNoise);
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("%d"), MapSize));
	Chunk->InitializeParameters(VoxelSize, HeightNoiseScale, ChunkSize, Depth, MapSize, HeightMap, HeatMap);
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
		Start = RenderRange * 2 - 1;
		Last = 0;
		Increment = -1;
		XShift = 0;
	}
	auto CurrentCoordinates = GetPlayerChunkCoordinates();
	int Shift = isLeft ? -RenderRange : RenderRange;
	for (int i = Start; Index > 0 ? i >= Last : i <= Last; i += Increment)
	{
		FVoxelLine* Line = &(*Map)[i];
		AVoxelChank* chunk = SpawnChunk((CurrentCoordinates.X + i - RenderRange + XShift) * ChunkLength,
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
	OctaveSharp = HeightParameters.OctaveSharp;
	OctaveSmooth = HeightParameters.OctaveSmooth;
	HeightZeroToOne = HeightParameters.ZeroToOne;
	HeightLacunarity = HeightParameters.Lacunarity;
	HeightPersistance = HeightParameters.Persistance;
	Multiplier = HeightParameters.Multiplier;
	ChunkSize = HeightParameters.ChunkSize;
	RenderRange = HeightParameters.RenderRange;
	Depth = HeightParameters.Depth;
	HeightNoiseDensity = HeightParameters.NoiseDensity;
	HeightNoiseScale = HeightParameters.NoiseScale;
	Biom = HeightParameters.Biom;
	VoxelSize = HeightParameters.VoxelSize;
	NoiseDensity3D = HeightParameters.NoiseDensity3D;
	Threshold3D = HeightParameters.Threshold3D;
	NoiseDensityTemperature = TemperatureParameters.NoiseDensity;
	TemperatureZeroToOne = TemperatureParameters.ZeroToOne;
	TemperatureLacunarity = TemperatureParameters.Lacunarity;
	PersistenceTemperature = TemperatureParameters.Persistence;
	if (Multiplier % 2 == 0)
		Multiplier += 1;
	MapSize = (ChunkSize * 2 + 1) * Multiplier;
	ChunkRenderLines = new FActorSpawnParameters();
	Map = new TArray<FVoxelLine>();
	for (int i = RenderRange * -1; i <= RenderRange; i++)
	{
		Map->Add(FVoxelLine());
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
	InitializeParameters();
	GenerateMaps();
	ChunkLength = ChunkSize * VoxelSize * 2 + VoxelSize;
	OldCoordinates = GetPlayerChunkCoordinates();
	for (int i = RenderRange * -1; i <= RenderRange; i++)
	{
		for (int j = RenderRange * -1; j <= RenderRange; j++)
		{
			FIntVector Vector = GetPlayerChunkCoordinates();
			int XCoord = (Vector.X + i) * ChunkLength;
			int YCoord = (Vector.Y + j) * ChunkLength;
			AVoxelChank* chunk = SpawnChunk(XCoord, YCoord, 0);
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


void AGenerationPlayerController::GenerateMaps()
{
	HeightMap = new float*[MapSize];
	HeatMap = new float*[MapSize];
	WaterMap = new float*[MapSize];
	MoistureMap = new float*[MapSize];
	const int LeftBorder = -(MapSize - 1) / 2;
	const int RightBorder = -LeftBorder;
	for (int i = LeftBorder; i <= RightBorder; i++)
	{
		int Index = i + RightBorder;
		HeatMap[Index] = new float[MapSize];
		HeightMap[Index] = new float[MapSize];
		WaterMap[Index] = new float[MapSize];
		MoistureMap[Index] = new float[MapSize];
	}
	GenerateHeatMap(LeftBorder, RightBorder);
	GenerateHeightMap(LeftBorder, RightBorder);
}

void AGenerationPlayerController::GenerateHeightMap(int LeftBorder, int RightBorder)
{
	USimplexNoiseBPLibrary::setNoiseSeed(16);
	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			const int XIndex = i + RightBorder;
			const int YIndex = j + RightBorder;
			float SharpNoise = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(
				i, -j, HeightLacunarity, HeightPersistance, OctaveSharp, HeightNoiseDensity, HeightZeroToOne);
			float SmoothNoise = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(
				i, -j, HeightLacunarity, HeightPersistance, OctaveSmooth, HeightNoiseDensity, HeightZeroToOne);
			SmoothNoise = Clamp(SmoothNoise, 0, 1);
			SharpNoise = Clamp(SharpNoise, 0, 1);
			TEnumAsByte<EBiomType> CurrentBiom = (TEnumAsByte<EBiomType>)AGenerationPlayerController::GetBiom(HeatMap[XIndex][YIndex]);
			const float FinalNoise = BezierComputations::FilterMap(SharpNoise, SmoothNoise, CurrentBiom);
			HeightMap[XIndex][YIndex] = FinalNoise;
		}
}

void AGenerationPlayerController::GenerateHeatMap(int LeftBorder, int RightBorder)
{
	USimplexNoiseBPLibrary::setNoiseSeed(21);
	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			float HeatNoise = USimplexNoiseBPLibrary::SimplexNoise2D(i, -j, NoiseDensityTemperature);
			HeatNoise = Clamp(HeatNoise, 0, 1);
			const int XIndex = i + RightBorder;
			const int YIndex = j + RightBorder;
			HeatMap[XIndex][YIndex] = HeatNoise;
		}
}

float AGenerationPlayerController::Clamp(float x, float left, float right)
{
	if (x < left)
	{
		return -x;
	}
	if (x > right)
		return right;
	return x;
}

uint8 AGenerationPlayerController::GetBiom(float Noise)
{
	uint8 bytes;
	if (Noise < 0.33)
	{
		bytes = (uint8)TUNDRA;
	}
	else if ((Noise >= 0.33) & (Noise < 0.66))
	{
		bytes = (uint8)TROPICAL_WOODLAND;
	}
	else
	{
		bytes = (uint8)DESERT;
	}
	return bytes;
}

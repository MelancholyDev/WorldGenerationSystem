#include "GenerationPlayerController.h"
#include <string>
#include "Math/BezierComputations.h"
#include "Kismet/GameplayStatics.h"
#include "Math/IntVector.h"
#include "Math/GausianFilter.h"
#include "SimplexNoiseBPLibrary.h"
#include "Math/DiamondSquare.h"
#include "Math/TestDiamond/DiamondSquareMapGeneration.h"

FIntVector AGenerationPlayerController::GetPlayerChunkCoordinates()
{
	FVector location = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	location = location / ChunkLength;
	return FIntVector(round(location.X), round(location.Y), 0);
}

void AGenerationPlayerController::GenerateTestMap()
{
	int SizeMult = 1;
	float Roughness = 0.5f;
	int Size = FMath::Pow(2,SizeMult)+1;
	float** Mass = new float*[Size];
	for (int i = 0; i < Size; i++)
	{
		Mass[i] = new float[Size];
	}
	DiamondSquareMapGeneration::generateWorld(Mass,Size);
	//DiamondSquare::GenerateMap(Mass, Size, Roughness);
	for (int i = 0; i < Size; i++)
	{
		FString String = "";
		for (int j = 0; j < Size; j++)
		{
			String += FString::Printf(TEXT("%f"), Mass[i][j]);
			String += " ";
		}
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, String);
	}
}

void AGenerationPlayerController::PrintCorners()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%f"),HeightMap[0][0]));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%f"),HeightMap[0][MapSize-1]));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%f"),HeightMap[MapSize-1][0]));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%f"),HeightMap[MapSize-1][MapSize-1]));
	
}


AVoxelChank* AGenerationPlayerController::SpawnChunk(float X, float Y, float Z)
{
	const FVector Location = {X, Y, Z};
	const FTransform Transform = FTransform(Location);
	AActor* NewActor = GetWorld()->SpawnActorDeferred<AVoxelChank>(AVoxelChank::StaticClass(), Transform);
	AVoxelChank* Chunk = Cast<AVoxelChank>(NewActor);
	FVoxelGenerationData Data;
	Data.Initialize(VoxelGenerationData.IsAddDepth, HeightParameters.VoxelSize, HeightParameters.NoiseScale,
	                HeightParameters.ChunkSize, HeightParameters.Depth, VoxelGenerationData.NoiseDensity3D,
	                VoxelGenerationData.Threshold3D, MapSize, HeightMap, HeatMap);
	Chunk->InitializeParameters(Data);
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
	int Shift = IsForward ? HeightParameters.RenderRange : -HeightParameters.RenderRange;
	FVoxelLine Line = FVoxelLine();
	for (int i = HeightParameters.RenderRange * -1; i <= HeightParameters.RenderRange; i++)
	{
		AVoxelChank* CreatedChunk = SpawnChunk((CurrentCoordinates.X + Shift) * ChunkLength,
		                                       (CurrentCoordinates.Y + i) * ChunkLength, 0);
		Line.Voxels.Add(CreatedChunk);
	}
	if (IsForward)
	{
		Map->Insert(Line, HeightParameters.RenderRange * 2);
	}
	else
	{
		Map->Insert(Line, 0);
	}
}

void AGenerationPlayerController::AddColumn(bool isLeft)
{
	auto CurrentCoordinates = GetPlayerChunkCoordinates();
	int Shift = isLeft ? -HeightParameters.RenderRange : HeightParameters.RenderRange;
	for (int i = 0; i <= HeightParameters.RenderRange * 2; i++)
	{
		AVoxelChank* CreatedChunk = SpawnChunk((CurrentCoordinates.X + i - HeightParameters.RenderRange) * ChunkLength,
		                                       (CurrentCoordinates.Y + Shift) * ChunkLength, 0);
		FVoxelLine* Line = &(*Map)[i];
		if (isLeft)
		{
			Line->Voxels.Insert(CreatedChunk, 0);
		}
		else
		{
			Line->Voxels.Insert(CreatedChunk, HeightParameters.RenderRange * 2);
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
		Last = HeightParameters.RenderRange * 2;
		Increment = 1;
		XShift = 0;
	}
	else
	{
		Start = HeightParameters.RenderRange * 2 - 1;
		Last = 0;
		Increment = -1;
		XShift = 0;
	}
	auto CurrentCoordinates = GetPlayerChunkCoordinates();
	int Shift = isLeft ? -HeightParameters.RenderRange : HeightParameters.RenderRange;
	for (int i = Start; Index > 0 ? i >= Last : i <= Last; i += Increment)
	{
		FVoxelLine* Line = &(*Map)[i];
		AVoxelChank* chunk = SpawnChunk(
			(CurrentCoordinates.X + i - HeightParameters.RenderRange + XShift) * ChunkLength,
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
	Multiplier = HeightParameters.Multiplier;
	if (Multiplier % 2 == 0)
		Multiplier += 1;
	if(GenerationParameters.GenerationType==PERLIN_NOISE)
	{
		MapSize = (HeightParameters.ChunkSize * 2 + 1) * Multiplier;
	}
	else
	{
		MapSize = FMath::Pow(2,GenerationParameters.DiamondMapSizeMultiplier)+1;
	}
	ChunkRenderLines = new FActorSpawnParameters();
	Map = new TArray<FVoxelLine>();
	for (int i = HeightParameters.RenderRange * -1; i <= HeightParameters.RenderRange; i++)
	{
		Map->Add(FVoxelLine());
	}
}

void AGenerationPlayerController::InitializeBiomData()
{
	TArray<FName> RowNames = DataTableBiome->GetRowNames();
	FString EmptyString;
	for (auto& Name : RowNames)
	{
		FBiomData Data = *DataTableBiome->FindRow<FBiomData>(Name, EmptyString);
		BiomDataSet.Add(Data.Type, Data);
	}
}

void AGenerationPlayerController::InitializeGausianKernel()
{
	GausianKernel = new float*[GausianParameters.KernelSize];
	for (int i = 0; i < GausianParameters.KernelSize; i++)
	{
		GausianKernel[i] = new float[GausianParameters.KernelSize];
	}
}

void AGenerationPlayerController::GetFullSize()
{
	for (int i = 0; i <= HeightParameters.RenderRange * 2; i++)
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
		DeleteLine(HeightParameters.RenderRange * 2);
		AddLine(false);
	}
}

void AGenerationPlayerController::YShift(int Y)
{
	if (Y < 0)
	{
		DeleteColumn(HeightParameters.RenderRange * 2);
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
			AppendColumn(HeightParameters.RenderRange * 2, false);
		}
		else
		{
			DeleteLine(0);
			DeleteColumn(HeightParameters.RenderRange * 2);
			AddLine(true);
			AppendColumn(HeightParameters.RenderRange * 2, true);
		}
	}
	else
	{
		if (Y > 0)
		{
			DeleteLine(HeightParameters.RenderRange * 2);
			DeleteColumn(0);
			AddLine(false);
			AppendColumn(0, false);
		}
		else
		{
			DeleteLine(HeightParameters.RenderRange * 2);
			DeleteColumn(HeightParameters.RenderRange * 2);
			AddLine(false);
			AppendColumn(0, true);
		}
	}
}

void AGenerationPlayerController::BeginPlay()
{
	InitializeParameters();
	InitializeBiomData();
	GenerateMaps();
	ChunkLength = HeightParameters.ChunkSize * HeightParameters.VoxelSize * 2 + HeightParameters.VoxelSize;
	OldCoordinates = GetPlayerChunkCoordinates();
	for (int i = HeightParameters.RenderRange * -1; i <= HeightParameters.RenderRange; i++)
	{
		for (int j = HeightParameters.RenderRange * -1; j <= HeightParameters.RenderRange; j++)
		{
			FIntVector Vector = GetPlayerChunkCoordinates();
			int XCoord = (Vector.X + i) * ChunkLength;
			int YCoord = (Vector.Y + j) * ChunkLength;
			AVoxelChank* chunk = SpawnChunk(XCoord, YCoord, 0);
			(*Map)[i + HeightParameters.RenderRange].Voxels.Add(chunk);
		}
	}
}

void AGenerationPlayerController::Tick(float DeltaSeconds)
{
	std::string s = std::to_string(HeightParameters.RenderRange);
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
	GausianKernel = new float*[MapSize];

	const int LeftBorder = -(MapSize - 1) / 2;
	const int RightBorder = -LeftBorder;

	for (int i = LeftBorder; i <= RightBorder; i++)
	{
		int Index = i + RightBorder;
		HeatMap[Index] = new float[MapSize];
		HeightMap[Index] = new float[MapSize];
		WaterMap[Index] = new float[MapSize];
		MoistureMap[Index] = new float[MapSize];
		GausianKernel[Index] = new float[MapSize];
	}
	GenerateHeatMap(LeftBorder, RightBorder);
	GenerateHeightMap(LeftBorder, RightBorder);
}

void AGenerationPlayerController::GenerateHeightMap(int LeftBorder, int RightBorder)
{
	InitializeGausianKernel();
	GausianFilter::CreateKernel(GausianKernel, GausianParameters.KernelSize, GausianParameters.Sigma);
	
	if (GenerationParameters.GenerationType == PERLIN_NOISE)
	{
		float** TempHeightMap = new float*[MapSize];
		for (int i = 0; i < MapSize; i++)
		{
			TempHeightMap[i] = new float[MapSize];
		}
		USimplexNoiseBPLibrary::setNoiseSeed(16);
		for (int i = LeftBorder; i <= RightBorder; i++)
			for (int j = RightBorder; j >= LeftBorder; j--)
			{
				const int XIndex = i + RightBorder;
				const int YIndex = j + RightBorder;
				float SharpNoise;
				float SmoothNoise;

				SharpNoise = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(
					i, -j, HeightParameters.Lacunarity, HeightParameters.Persistance, HeightParameters.OctaveSharp,
					HeightParameters.NoiseDensity, HeightParameters.ZeroToOne);

				SmoothNoise = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(
					i, -j, HeightParameters.Lacunarity, HeightParameters.Persistance, HeightParameters.OctaveSmooth,
					HeightParameters.NoiseDensity, HeightParameters.ZeroToOne);

				SmoothNoise = Clamp(SmoothNoise, 0, 1);
				SharpNoise = Clamp(SharpNoise, 0, 1);

				TEnumAsByte<EBiomType> CurrentBiom;
				if (HeightParameters.IsTest)
				{
					CurrentBiom = (TEnumAsByte<EBiomType>)GetBiom(HeatMap[XIndex][YIndex]);
				}
				else
				{
					CurrentBiom = HeightParameters.Biom;
				}
				FBiomData* CurrentBiomData = BiomDataSet.Find(CurrentBiom);
				float FinalNoise = BezierComputations::FilterMap(SharpNoise, SmoothNoise, *CurrentBiomData);
				float Clamped = Clamp(FinalNoise, 0, 1);
				CurrentBiomData->CheckValue(Clamped);
				TempHeightMap[XIndex][YIndex] = Clamped;
			}
		for (int i = 0; i < MapSize; i++)
		{
			if ((i == 0) | (i + 1 == MapSize))
				for (int j = 0; j < MapSize; j++)
				{
					HeightMap[i][j] = TempHeightMap[i][j];
				}
			else
			{
				HeightMap[i][0] = TempHeightMap[i][0];
				HeightMap[i][MapSize - 1] = TempHeightMap[i][MapSize - 1];
			}
		}
		if (isInvert)
		{
			InvertMap(TempHeightMap, LeftBorder, RightBorder);
		}
		if (GausianParameters.IsApplyGausianFilter)
		{
			GausianFilter::SmoothMap(TempHeightMap, MapSize, HeightMap, GausianKernel, GausianParameters.KernelSize);
		}
		else
		{
			HeightMap = TempHeightMap;
		}
	}
	else
	{
		//DiamondSquare::GenerateMap(HeightMap, MapSize, GenerationParameters.Roughness);
		DiamondSquareMapGeneration::generateWorld(HeightMap,MapSize);
	}
}

void AGenerationPlayerController::GenerateHeatMap(int LeftBorder, int RightBorder)
{
	USimplexNoiseBPLibrary::setNoiseSeed(21);
	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			float HeatNoise = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(
				i, -j, TemperatureParameters.Lacunarity, TemperatureParameters.Persistence,
				TemperatureParameters.Octaves, TemperatureParameters.NoiseDensity, TemperatureParameters.ZeroToOne);
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

void AGenerationPlayerController::InvertMap(float** MapForInvert, int LeftBorder, int RightBorder)
{
	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			const int XIndex = i + RightBorder;
			const int YIndex = j + RightBorder;
			TEnumAsByte<EBiomType> CurrentBiom;
			if (HeightParameters.IsTest)
			{
				CurrentBiom = (TEnumAsByte<EBiomType>)GetBiom(HeatMap[XIndex][YIndex]);
			}
			else
			{
				CurrentBiom = HeightParameters.Biom;
			}
			FBiomData* CurrentBiomData = BiomDataSet.Find(CurrentBiom);
			float Noise = MapForInvert[XIndex][YIndex];
			Noise = CurrentBiomData->Max - (Noise - CurrentBiomData->Min);
			MapForInvert[XIndex][YIndex] = Noise;
		}
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

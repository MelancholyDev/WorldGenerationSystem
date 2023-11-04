#pragma once

#include "CoreMinimal.h"
#include "VoxelChank.h"
#include "Classes/Generator.h"
#include "Data/WorldGenerationSettings.h"
#include "Structures/FVoxelLine.h"
#include "Structures/FVoxelGeneraionData.h"
#include "WorldGenerationSubsystem.generated.h"

UCLASS()
class SURVIVALGAME_API UWorldGenerationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void GenerateWorld(UWorldGenerationSettings* WorldGenerationSystem);
	
private:
	
	void GetFullSize();
	void XShift(int X);
	void YShift(int Y);
	void Diagonal(int X, int Y);
	void DeleteLine(int Index);
	void AddLine(bool IsForward);
	void AddColumn(bool isLeft);
	void AppendColumn(int Index, bool isLeft);
	void DeleteColumn(int Index);
	void InitializeParameters();
	void GenerateMaps();
	void GenerateHeightMap() const;
	void GenerateBiomMap() const;
	void GenerateCaveMap() const;

	AVoxelChank* SpawnChunk(float X, float Y, float Z);
	FIntVector GetPlayerChunkCoordinates();

private:
	UWorldGenerationSettings* GenerationSettings;
	int MapSize;
	int ChunkLength;
	FIntVector OldCoordinates;
	TArray<FVoxelLine>* Map;
	FActorSpawnParameters* ChunkRenderLines;
	Generator* GeneratorInstance;
	FVoxelGenerationData VoxelGenerationData;

	float** HeightMap;
	EBiomType** BiomMap;
	float** WaterMap;
	float*** UndergroundMap;
	
};

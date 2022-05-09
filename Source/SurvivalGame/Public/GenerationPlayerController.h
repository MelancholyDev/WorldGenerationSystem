#pragma once

#include "CoreMinimal.h"
#include "Enums/EBiomType.h"
#include "VoxelChank.h"
#include "Classes/Generator.h"
#include "GameFramework/PlayerController.h"
#include "Structures/FPerlinNoiseParameters.h"
#include "Structures/FTemperatureParameter.h"
#include "Factories/CompositeDataTableFactory.h"
#include "Structures/FBiomData.h"
#include "Structures/FVoxelLine.h"
#include "Structures/FGausianParameters.h"
#include "Structures/FVoxelGeneraionData.h"
#include "Structures/FGenerationParameters.h"
#include "GenerationPlayerController.generated.h"

UCLASS()
class SURVIVALGAME_API AGenerationPlayerController : public APlayerController
{
	GENERATED_BODY()
	int MapSize;
	int ChunkLength;
	FIntVector OldCoordinates;
	TArray<FVoxelLine>* Map;
	FActorSpawnParameters* ChunkRenderLines;
	
	Generator* GeneratorInstance;
	float** HeightMap;
	float** HeatMap;
	float** MoistureMap;
	float** WaterMap;
	
	UPROPERTY(EditAnywhere)
	FGenerationParameters GenerationParameters;
	UPROPERTY(EditAnywhere)
	FVoxelGenerationData VoxelGenerationData;
	UPROPERTY(EditAnywhere)
	UDataTable* BiomDataSet;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AVoxelChank> ToSpawn;
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable)
	void PrintFullMap();
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
	void GenerateHeightMap();
	void GenerateHeatMap();
	
	AVoxelChank* SpawnChunk(float X, float Y, float Z);
	FIntVector GetPlayerChunkCoordinates();
};

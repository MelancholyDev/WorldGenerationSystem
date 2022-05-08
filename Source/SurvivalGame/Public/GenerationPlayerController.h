#pragma once

#include "CoreMinimal.h"
#include "Enums/EBiomType.h"
#include "VoxelChank.h"
#include "GameFramework/PlayerController.h"
#include "Structures/FHeightParameters.h"
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
	int Multiplier;
	int MapSize;
	int ChunkLength;
	UPROPERTY(EditAnywhere)
	bool isInvert;
	FIntVector OldCoordinates;
	TArray<FVoxelLine>* Map;
	FActorSpawnParameters* ChunkRenderLines;
	
	float** HeightMap;
	float** HeatMap;
	float** MoistureMap;
	float** WaterMap;
	float** GausianKernel;
	
	UPROPERTY(EditAnywhere)
	FGenerationParameters GenerationParameters;
	UPROPERTY(EditAnywhere)
	FGausianParameters GausianParameters;
	UPROPERTY(EditAnywhere)
	FHeightParameters HeightParameters;
	UPROPERTY(EditAnywhere)
	FTemperatureParameters TemperatureParameters;
	UPROPERTY(EditAnywhere)
	FVoxelGenerationData VoxelGenerationData;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AVoxelChank> ToSpawn;

	TMap<EBiomType,FBiomData> BiomDataSet;
	UPROPERTY(EditAnywhere)
	UDataTable* DataTableBiome;
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
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
	void InitializeBiomData();
	void InitializeGausianKernel();

	uint8 GetBiom(float Noise);
	float Clamp(float x, float left, float right);
	void InvertMap(float** Map,int LeftBorder, int RightBorder);
	void GenerateMaps();
	void GenerateHeightMap(int LeftBorder, int RightBorder);
	void GenerateHeatMap(int LeftBorder, int RightBorder);
	
	AVoxelChank* SpawnChunk(float X, float Y, float Z);
	FIntVector GetPlayerChunkCoordinates();

	UFUNCTION(BlueprintCallable)
	void GenerateTestMap();
	UFUNCTION(BlueprintCallable)
	void PrintCorners();
};

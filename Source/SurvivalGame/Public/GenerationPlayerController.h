// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enums.h"
#include "VoxelChank.h"
#include "GameFramework/PlayerController.h"
#include "Structures.h"
#include "Enums.h"
#include "GenerationPlayerController.generated.h"


UCLASS()
class SURVIVALGAME_API AGenerationPlayerController : public APlayerController
{
	GENERATED_BODY()

	int OctaveSharp;
	int OctaveSmooth;
	bool HeightZeroToOne;
	float HeightLacunarity;
	float HeightPersistance;
	int Multiplier;
	int MapSize;
	float ChunkSize;
	float ChunkLength;
	int RenderRange;
	int Depth;
	float HeightNoiseDensity;
	float HeightNoiseScale;
	TEnumAsByte<EBiomType> Biom;;
	float VoxelSize;
	float NoiseDensity3D;
	float Threshold3D;
	int WaterLevel;
	
	float NoiseDensityTemperature;
	float TemperatureLacunarity;
	float PersistenceTemperature;
	bool TemperatureZeroToOne;
	
	FIntVector OldCoordinates;
	TArray<FVoxelLine>* Map;
	FActorSpawnParameters* ChunkRenderLines;
	float** HeightMap;
	float** HeatMap;
	float** MoistureMap;
	float** WaterMap;
	
	UPROPERTY(EditAnywhere)
	FHeightParameters HeightParameters;
	UPROPERTY(EditAnywhere)
	FTemperatureParameters TemperatureParameters;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AVoxelChank> ToSpawn;
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
private:
	uint8 GetBiom(float Noise);
	float Clamp(float x, float left, float right);
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
	void GenerateHeightMap(int LeftBorder, int RightBorder);
	void GenerateHeatMap(int LeftBorder, int RightBorder);
	AVoxelChank* SpawnChunk(float X, float Y, float Z);
	FIntVector GetPlayerChunkCoordinates();
};

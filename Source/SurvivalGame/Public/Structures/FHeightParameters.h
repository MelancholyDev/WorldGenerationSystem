#pragma once
#include "Enums/EBiomType.h"
#include "FHeightParameters.generated.h"

USTRUCT()
struct FHeightParameters
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	int OctaveSharp;
	UPROPERTY(EditAnywhere)
	int OctaveSmooth;
	UPROPERTY(EditAnywhere)
	bool ZeroToOne;
	UPROPERTY(EditAnywhere)
	float Lacunarity;
	UPROPERTY(EditAnywhere)
	float Persistance;
	UPROPERTY(EditAnywhere)
	int Multiplier;
	UPROPERTY(EditAnywhere)
	int ChunkSize;
	UPROPERTY(EditAnywhere)
	int RenderRange;
	UPROPERTY(EditAnywhere)
	int Depth;
	UPROPERTY(EditAnywhere)
	float NoiseDensity;
	UPROPERTY(EditAnywhere)
	int NoiseScale;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EBiomType> Biom;
	UPROPERTY(EditAnywhere)
	bool IsTest;
	UPROPERTY(EditAnywhere)
	int VoxelSize;
};
#pragma once
#include "CoreMinimal.h"
#include "VoxelChank.h"
#include "Enums.h"
#include "Structures.generated.h"
USTRUCT()
struct FVoxelLine
{
	GENERATED_BODY();
	TArray<AVoxelChank*> Voxels;
	FVoxelLine()
	{
		Voxels.Empty();
	}
};

USTRUCT()
struct FTemperatureParameters
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere)
	int Octaves;
	UPROPERTY(EditAnywhere)
	float Lacunarity=0.1;
	UPROPERTY(EditAnywhere)
	float Persistence=0.1;
	UPROPERTY(EditAnywhere)
	float NoiseDensity=0.1;
	UPROPERTY(EditAnywhere)
	bool ZeroToOne;
};

USTRUCT()
struct FHeightParameters{
	GENERATED_BODY();
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
	TEnumAsByte<BiomType> Biom;
	UPROPERTY(EditAnywhere)
	int VoxelSize;
	UPROPERTY(EditAnywhere)
	float NoiseDensity3D;
	UPROPERTY(EditAnywhere)
	float Threshold3D;
};


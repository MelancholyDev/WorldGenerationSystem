#pragma once
#include "CoreMinimal.h"
#include "VoxelChank.h"
#include "Enums.h"
#include "Engine/DataTable.h"

#include "Structures.generated.h"
USTRUCT()
struct FVoxelLine
{
	GENERATED_BODY()
	TArray<AVoxelChank*> Voxels;

	FVoxelLine()
	{
		Voxels.Empty();
	}
};

USTRUCT()
struct FTemperatureParameters
{
	GENERATED_BODY()
	;
	UPROPERTY(EditAnywhere)
	int Octaves;
	UPROPERTY(EditAnywhere)
	float Lacunarity = 0.1;
	UPROPERTY(EditAnywhere)
	float Persistence = 0.1;
	UPROPERTY(EditAnywhere)
	float NoiseDensity = 0.1;
	UPROPERTY(EditAnywhere)
	bool ZeroToOne;
};

USTRUCT()
struct FHeightParameters
{
	GENERATED_BODY()
	;
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
	int VoxelSize;
	UPROPERTY(EditAnywhere)
	float NoiseDensity3D;
	UPROPERTY(EditAnywhere)
	float Threshold3D;
};

USTRUCT()
struct FGausianParameters
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	bool IsApplyGausianFilter;
	UPROPERTY(EditAnywhere)
	int KernelSize;
	UPROPERTY(EditAnywhere)
	int Sigma;
};

USTRUCT(BlueprintType)
struct FBiomData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EBiomType> Type;
	UPROPERTY(EditAnywhere)
	int Max = 0;
	UPROPERTY(EditAnywhere)
	int Min = 1;
	UPROPERTY(EditAnywhere)
	float B;
	UPROPERTY(EditAnywhere)
	float A;
	UPROPERTY(EditAnywhere)
	float X1;
	UPROPERTY(EditAnywhere)
	float Y1;
	UPROPERTY(EditAnywhere)
	float X2;
	UPROPERTY(EditAnywhere)
	float Y2;

	void CheckValue(int Number)
	{
		if (Number < Min)
		{
			Min = Number;
		}
		if (Number > Max)
		{
			Max = Number;
		}
	}
};
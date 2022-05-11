#pragma once
#include "FBiomMapGenerateParameters.h"
#include "FDiamondSquareParameters.h"
#include "FGausianParameters.h"
#include "FPerlinNoiseParameters.h"
#include "FTemperatureAndMoistureParameters.h"
#include "Enums/EGenerationType.h"
#include "FGenerationParameters.generated.h"

USTRUCT()
struct FGenerationParameters
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EGenerationType> GenerationType;
	UPROPERTY(EditAnywhere)
	bool IsApplyGausianFilter;
	UPROPERTY(EditAnywhere)
	bool IsAddBezierFunction;
	UPROPERTY(EditAnywhere)
	int RenderRange;
	UPROPERTY(EditAnywhere)
	int ChunkSize;
	UPROPERTY(EditAnywhere)
	int VoxelSize;
	UPROPERTY(EditAnywhere)
	FDiamondSquareParameters DiamondSquareParameters;
	UPROPERTY(EditAnywhere)
	FPerlinNoiseParameters PerlinNoiseParameters;
	UPROPERTY(EditAnywhere)
	FGausianParameters GausianParameters;
	UPROPERTY(EditAnywhere)
	FTemperatureAndMoistureParameters TemperatureAndMoistureParameters;
};

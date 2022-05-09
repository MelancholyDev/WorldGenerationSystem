#pragma once
#include "FDiamondSquareParameters.h"
#include "FGausianParameters.h"
#include "FPerlinNoiseParameters.h"
#include "FTemperatureParameter.h"
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
	FDiamondSquareParameters DiamondSquareParameters;
	UPROPERTY(EditAnywhere)
	FPerlinNoiseParameters PerlinNoiseParameters;
	UPROPERTY(EditAnywhere)
	FGausianParameters GausianParameters;
	UPROPERTY(EditAnywhere)
	FTemperatureParameters TemperatureParameters;
	UPROPERTY(EditAnywhere)
	int RenderRange;
	UPROPERTY(EditAnywhere)
	int ChunkSize;
	UPROPERTY(EditAnywhere)
	int VoxelSize;
};

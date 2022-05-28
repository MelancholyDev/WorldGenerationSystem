#pragma once
#include "FPerlinParameters.h"
#include "FTemperatureAndMoistureParameters.generated.h"

USTRUCT()
struct FTemperatureAndMoistureParameters
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FPerlinParameters TemperatureParameters;
	UPROPERTY(EditAnywhere)
	FPerlinParameters MoistureParameters;
};
#pragma once
#include "FTemperatureParameter.generated.h"

USTRUCT()
struct FTemperatureParameters
{
	GENERATED_BODY()
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
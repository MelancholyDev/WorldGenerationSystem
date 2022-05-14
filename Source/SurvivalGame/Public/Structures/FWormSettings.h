#pragma once
#include "FWormSettings.generated.h"

USTRUCT()
struct FWormSettings
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	float Persistance;
	UPROPERTY(EditAnywhere)
	float Octaves;
	UPROPERTY(EditAnywhere)
	float NoiseDensity;
	UPROPERTY(EditAnywhere)
	float Lacunarity;
	UPROPERTY(EditAnywhere)
	float StartFrequency;

};

#pragma once
#include "FCaveDistibution.generated.h"

USTRUCT()
struct FCaveDistibution
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere)
	float Lacunarity;
	UPROPERTY(EditAnywhere)
	float Persistance;
	UPROPERTY(EditAnywhere)
	int Octaves;
	UPROPERTY(EditAnywhere)
	float NoiseDensity3D;
	UPROPERTY(EditAnywhere)
	bool ZeroToOne;
};

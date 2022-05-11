#pragma once
#include "Enums/EBiomType.h"
#include "FPerlinNoiseParameters.generated.h"

USTRUCT()
struct FPerlinNoiseParameters
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	int Seed;
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
	float NoiseDensity;
	UPROPERTY(EditAnywhere)
	int NoiseScale;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EBiomType> Biom;
	UPROPERTY(EditAnywhere)
	bool IsTest;
	UPROPERTY(EditAnywhere)
	bool IsInvert;
};
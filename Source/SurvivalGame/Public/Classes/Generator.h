#pragma once
#include "Math/BezierComputations.h"
#include "Math/DiamondSquare.h"
#include "Math/GausianFilter.h"
#include "Structures/FPerlinNoiseParameters.h"
#include "Structures/FTemperatureParameter.h"

class Generator
{
public:
	Generator(FGenerationParameters Parameters, UDataTable* Table);
	void GenerateHeightMap(float** Map, float** HeatMap);
	void GenerateBiomMaps(float** TemperatureMap,float** MoistureMap);
	void GenerateSeaMap(float** Map);
private:
	void InitializeBiomData();
	void GenerateWithPerlinNoise(float** Map, float** HeatMap);
	void GenerateWithDiamondSquare(float** Map);
	float Clamp(float x, float left, float right);
	uint8 GetBiom(float Noise);
	void InvertMap(float** MapForInvert, float** HeatMap);

	FGenerationParameters GenerationParameters;
	BezierComputations* BezierComputationsInstance;
	DiamondSquare* DiamondSquareInstance;
	GausianFilter* GausianFilterInstance;

	FGausianParameters GausianParameters;
	FPerlinNoiseParameters PerlinNoiseParameters;
	FTemperatureParameters TemperatureParameters;
	FDiamondSquareParameters DiamondSquareParameters;

	TMap<EBiomType, FBiomData> BiomDataSet;
	UDataTable* DataTableBiome;

	int LeftBorder;
	int RightBorder;
	int MapSize;
};
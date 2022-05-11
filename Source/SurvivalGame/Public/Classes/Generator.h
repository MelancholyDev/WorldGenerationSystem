#pragma once
#include "Math/BezierComputations.h"
#include "Math/DiamondSquare.h"
#include "Math/GausianFilter.h"
#include "Structures/FPerlinNoiseParameters.h"
#include "Structures/FTemperatureAndMoistureParameters.h"

class Generator
{
public:
	Generator(FGenerationParameters Parameters, UDataTable* Table);
	void GenerateHeightMap(float** Map, float** HeatMap,float** MoistureMap);
	void GenerateBiomMaps(float** TemperatureMap,float** MoistureMap);
	void GenerateSeaMap(float** Map);
private:
	void InitializeBiomData();
	void GenerateWithPerlinNoise(float** Map, float** HeatMap,float** MoistureMap);
	void GenerateWithDiamondSquare(float** Map);
	float Clamp(float x, float left, float right);
	uint8 GetBiom(float Heat,float Moisture);
	void InvertMap(float** MapForInvert, float** HeatMap,float** MoistureMap);

	FGenerationParameters GenerationParameters;
	BezierComputations* BezierComputationsInstance;
	DiamondSquare* DiamondSquareInstance;
	GausianFilter* GausianFilterInstance;

	FGausianParameters GausianParameters;
	FPerlinNoiseParameters PerlinNoiseParameters;
	
	FBiomMapGenerateParameters MoistureParameters;
	FBiomMapGenerateParameters TemperatureParameters;
	
	FDiamondSquareParameters DiamondSquareParameters;

	TMap<EBiomType, FBiomData> BiomDataSet;
	UDataTable* DataTableBiome;

	int LeftBorder;
	int RightBorder;
	int MapSize;
};
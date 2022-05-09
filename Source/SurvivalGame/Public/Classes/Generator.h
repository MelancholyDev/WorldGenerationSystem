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
	void GenerateTemperatureMap(float** Map);
	void GenerateMoistureMap(float** Map);
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

inline void Generator::GenerateWithDiamondSquare(float** Map)
{
	DiamondSquareInstance->GenerateMap(Map);
	if (GenerationParameters.IsApplyGausianFilter)
	{
		float** TempMap = new float*[MapSize];
		for (int i = 0; i < MapSize; i++)
		{
			TempMap[i]=new float[MapSize];
			for (int j = 0; j < MapSize; j++)
				TempMap[i][j] = Map[i][j];
		}
		GausianFilterInstance->SmoothMap(TempMap, Map);
	}
}

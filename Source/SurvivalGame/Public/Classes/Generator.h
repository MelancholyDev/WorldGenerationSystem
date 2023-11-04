#pragma once
#include "Data/WorldGenerationSettings.h"
#include "Math/BezierComputations.h"
#include "Math/DiamondSquare.h"
#include "Math/GausianFilter.h"
#include "Math/PerlinWormGenerator.h"
#include "Structures/FPerlinNoiseParameters.h"
#include "Structures/FUndergroundParameters.h"
#include "Structures/FVoxelGeneraionData.h"
#include "Structures/FWormSettings.h"

class Generator
{
public:
	Generator(UWorldGenerationSettings* GenerationSettings);
	Generator();
	void SetGenerationSettings(UWorldGenerationSettings* GenerationSettings);
	void GenerateHeightMap(float** Map, EBiomType** BiomMap);
	void GenerateBiomMaps(EBiomType** BiomMap);
	void GenerateSeaMap(float** Map);
	void GenerateCaveMap(float*** UndergroundMap);
private:
	void CreateInstances();
	void InitializeBiomData();
	void GenerateWithPerlinNoise(float** Map, EBiomType** BiomMap);
	void GenerateWithDiamondSquare(float** Map);
	float Clamp(float x, float left, float right);
	uint8 GetBiom(float Heat, float Moisture);
	void InvertMap(float** MapForInvert, EBiomType** BiomMap);

	UWorldGenerationSettings* WorldGenerationSettings;
	TMap<EBiomType, FBiomData> BiomDataSet;

	//Computation instances
	BezierComputations* BezierComputationsInstance;
	DiamondSquare* DiamondSquareInstance;
	GausianFilter* GausianFilterInstance;
	PerlinWormGenerator* WormGenerator;
	
	int LeftBorder;
	int RightBorder;
	int MapSize;
	int StartCaveIndex;
};

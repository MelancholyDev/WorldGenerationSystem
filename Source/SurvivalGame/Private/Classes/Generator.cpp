#include "Classes/Generator.h"

#include "PerlinNoiseBPLibrary.h"
#include "Structures/FWormSettings.h"

Generator::Generator(UWorldGenerationSettings* GenerationSettings)
{
	SetGenerationSettings(GenerationSettings);
	CreateInstances();
}

Generator::Generator()
{
	CreateInstances();
}

void Generator::SetGenerationSettings(UWorldGenerationSettings* GenerationSettings)
{
	WorldGenerationSettings = GenerationSettings;
	InitializeBiomData();

	if (WorldGenerationSettings->GenerationType == PERLIN_NOISE)
	{
		MapSize = (WorldGenerationSettings->ChunkSize * 2 + 1) * WorldGenerationSettings->PerlinNoiseParameters.
			MapSizeMultiplier;
	}
	else
	{
		MapSize = FMath::Pow(2, WorldGenerationSettings->DiamondSquareParameters.MapMultiplier) + 1;
	}
	LeftBorder = -(MapSize - 1) / 2;
	RightBorder = -LeftBorder;
}

float Generator::Clamp(float x, float left, float right)
{
	if (x < left)
	{
		return -x;
	}
	if (x > right)
		return right;
	return x;
}


void Generator::CreateInstances()
{
	BezierComputationsInstance = new BezierComputations(BiomDataSet);
	DiamondSquareInstance = new DiamondSquare(WorldGenerationSettings->DiamondSquareParameters, MapSize);
	GausianFilterInstance = new GausianFilter(WorldGenerationSettings->GausianParameters, MapSize);
	WormGenerator = new PerlinWormGenerator(
		MapSize,
		WorldGenerationSettings->UndergroundParameters.CaveStart - WorldGenerationSettings->UndergroundParameters.Depth
		+ 1, WorldGenerationSettings->UndergroundParameters.WormSettings);
}

void Generator::GenerateHeightMap(float** Map, EBiomType** BiomMap)
{
	switch (WorldGenerationSettings->GenerationType)
	{
	case PERLIN_NOISE:
		{
			GenerateWithPerlinNoise(Map, BiomMap);
		}
		break;
	case DIAMOND_SQUARE:
		{
			GenerateWithDiamondSquare(Map);
		}
		break;
	default: ;
	}
}

void Generator::GenerateBiomMaps(EBiomType** BiomMap)
{
	float** TemperatureMap = new float*[MapSize];
	float** MoistureMap = new float*[MapSize];

	for (int i = 0; i < MapSize; i++)
	{
		MoistureMap[i] = new float[MapSize];
		TemperatureMap[i] = new float[MapSize];
	}

	USimplexNoiseBPLibrary::createSeed(WorldGenerationSettings->TemperatureParameters.Seed);
	USimplexNoiseBPLibrary::setNoiseSeed(WorldGenerationSettings->TemperatureParameters.Seed);

	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			float HeatNoise = USimplexNoiseBPLibrary::PerlinNoise2D_EX(
				i, -j, WorldGenerationSettings->TemperatureParameters.Lacunarity,
				WorldGenerationSettings->TemperatureParameters.Persistence,
				WorldGenerationSettings->TemperatureParameters.Octaves,
				WorldGenerationSettings->TemperatureParameters.NoiseDensity,
				WorldGenerationSettings->TemperatureParameters.ZeroToOne);
			HeatNoise = Clamp(HeatNoise, 0, 1);
			const int XIndex = i + RightBorder;
			const int YIndex = j + RightBorder;

			TemperatureMap[XIndex][YIndex] = HeatNoise;
		}

	USimplexNoiseBPLibrary::createSeed(WorldGenerationSettings->MoistureParameters.Seed);
	USimplexNoiseBPLibrary::setNoiseSeed(WorldGenerationSettings->MoistureParameters.Seed);

	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			float MoistureNoise = USimplexNoiseBPLibrary::PerlinNoise2D_EX(
				i, -j, WorldGenerationSettings->MoistureParameters.Lacunarity,
				WorldGenerationSettings->MoistureParameters.Persistence,
				WorldGenerationSettings->MoistureParameters.Octaves,
				WorldGenerationSettings->MoistureParameters.NoiseDensity,
				WorldGenerationSettings->MoistureParameters.ZeroToOne);
			MoistureNoise = Clamp(MoistureNoise, 0, 1);

			const int XIndex = i + RightBorder;
			const int YIndex = j + RightBorder;

			MoistureMap[XIndex][YIndex] = MoistureNoise;
		}

	for (int i = 0; i < MapSize; i++)
		for (int j = 0; j < MapSize; j++)
		{
			float MoistureNoise = MoistureMap[i][j];
			float TemperatureNoise = TemperatureMap[i][j];

			BiomMap[i][j] = (EBiomType)GetBiom(TemperatureNoise, MoistureNoise);
		}
}


void Generator::GenerateSeaMap(float** Map)
{
}

void Generator::GenerateCaveMap(float*** UndergroundMap)
{
	int Depth = WorldGenerationSettings->UndergroundParameters.CaveStart - WorldGenerationSettings->
	                                                                       UndergroundParameters.Depth + 1;
	float*** FirstNoise = new float**[MapSize];
	float*** SecondNoise = new float**[MapSize];
	float*** CavePositions = new float**[MapSize];

	for (int i = 0; i < MapSize; i++)
	{
		FirstNoise[i] = new float*[MapSize];
		CavePositions[i] = new float*[MapSize];
		SecondNoise[i] = new float*[MapSize];
		for (int j = 0; j < MapSize; j++)
		{
			FirstNoise[i][j] = new float[Depth];
			CavePositions[i][j] = new float[Depth];
			SecondNoise[i][j] = new float[Depth];
		}
	}
	USimplexNoiseBPLibrary::createSeed(WorldGenerationSettings->UndergroundParameters.FirstNoise.Seed);
	USimplexNoiseBPLibrary::setNoiseSeed(WorldGenerationSettings->UndergroundParameters.FirstNoise.Seed);

	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			int DepthIndex = 0;
			for (int k = WorldGenerationSettings->UndergroundParameters.CaveStart; k >= WorldGenerationSettings->
			     UndergroundParameters.Depth; k--)
			{
				float Noise3D = USimplexNoiseBPLibrary::PerlinNoise3D_EX(
					i, -j, k, WorldGenerationSettings->UndergroundParameters.FirstNoise.Lacunarity,
					WorldGenerationSettings->UndergroundParameters.FirstNoise.Persistence,
					WorldGenerationSettings->UndergroundParameters.FirstNoise.Octaves,
					WorldGenerationSettings->UndergroundParameters.FirstNoise.NoiseDensity,
					WorldGenerationSettings->UndergroundParameters.FirstNoise.ZeroToOne);
				const int XIndex = i + RightBorder;
				const int YIndex = j + RightBorder;
				FirstNoise[XIndex][YIndex][DepthIndex] = Clamp(Noise3D, 0, 1);
				DepthIndex++;
			}
		}

	USimplexNoiseBPLibrary::createSeed(WorldGenerationSettings->UndergroundParameters.SecondNoise.Seed);
	USimplexNoiseBPLibrary::setNoiseSeed(WorldGenerationSettings->UndergroundParameters.SecondNoise.Seed);

	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			int DepthIndex = 0;
			for (int k = WorldGenerationSettings->UndergroundParameters.CaveStart; k >= WorldGenerationSettings->
			     UndergroundParameters.Depth; k--)
			{
				float Noise3D = USimplexNoiseBPLibrary::PerlinNoise3D_EX(
					i, -j, k, WorldGenerationSettings->UndergroundParameters.SecondNoise.Lacunarity,
					WorldGenerationSettings->UndergroundParameters.SecondNoise.Persistence,
					WorldGenerationSettings->UndergroundParameters.SecondNoise.Octaves,
					WorldGenerationSettings->UndergroundParameters.SecondNoise.NoiseDensity,
					WorldGenerationSettings->UndergroundParameters.SecondNoise.ZeroToOne);
				const int XIndex = i + RightBorder;
				const int YIndex = j + RightBorder;
				SecondNoise[XIndex][YIndex][DepthIndex] = Clamp(Noise3D, 0, 1);
				DepthIndex++;
			}
		}
	USimplexNoiseBPLibrary::createSeed(WorldGenerationSettings->UndergroundParameters.WormPlaceNoise.Seed);
	USimplexNoiseBPLibrary::setNoiseSeed(WorldGenerationSettings->UndergroundParameters.WormPlaceNoise.Seed);

	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			int DepthIndex = 0;
			for (int k = WorldGenerationSettings->UndergroundParameters.CaveStart; k >= WorldGenerationSettings->
			     UndergroundParameters.Depth; k--)
			{
				float Noise3D = USimplexNoiseBPLibrary::PerlinNoise3D_EX(
					i, -j, k, WorldGenerationSettings->UndergroundParameters.WormPlaceNoise.Lacunarity,
					WorldGenerationSettings->UndergroundParameters.WormPlaceNoise.Persistence,
					WorldGenerationSettings->UndergroundParameters.WormPlaceNoise.Octaves,
					WorldGenerationSettings->UndergroundParameters.WormPlaceNoise.NoiseDensity,
					WorldGenerationSettings->UndergroundParameters.WormPlaceNoise.ZeroToOne);
				const int XIndex = i + RightBorder;
				const int YIndex = j + RightBorder;
				CavePositions[XIndex][YIndex][DepthIndex] = Clamp(Noise3D, 0, 1);
				DepthIndex++;
			}
		}

	WormGenerator->GenerateCaves(UndergroundMap, FirstNoise, SecondNoise, CavePositions);
}

void Generator::InitializeBiomData()
{
	TArray<FName> RowNames = WorldGenerationSettings->BiomDataSet->GetRowNames();
	FString EmptyString;
	for (auto& Name : RowNames)
	{
		FBiomData Data = *WorldGenerationSettings->BiomDataSet->FindRow<FBiomData>(Name, EmptyString);
		BiomDataSet.Add(Data.Type, Data);
	}
}

void Generator::GenerateWithDiamondSquare(float** Map)
{
	DiamondSquareInstance->GenerateMap(Map);
	if (WorldGenerationSettings->IsApplyGausianFilter)
	{
		float** TempMap = new float*[MapSize];
		for (int i = 0; i < MapSize; i++)
		{
			TempMap[i] = new float[MapSize];
			for (int j = 0; j < MapSize; j++)
				TempMap[i][j] = Map[i][j];
		}
		GausianFilterInstance->SmoothMap(TempMap, Map);
	}
}

uint8 Generator::GetBiom(float Heat, float Moisture)
{
	uint8 bytes;
	if (Heat < 0.33)
	{
		if (Moisture < 0.33)
		{
			bytes = (uint8)BOREAL;
		}
		else if ((Moisture >= 0.33) & (Moisture < 0.66))
		{
			bytes = (uint8)SEASONAL_FOREST;
		}
		else
		{
			bytes = (uint8)TUNDRA;
		}
	}
	else if ((Heat >= 0.33) & (Heat < 0.66))
	{
		if (Moisture < 0.33)
		{
			bytes = (uint8)TEMPERATE_FOREST;
		}
		else if ((Moisture >= 0.33) & (Moisture < 0.66))
		{
			bytes = (uint8)TROPICAL_WOODLAND;
		}
		else
		{
			bytes = (uint8)TEMPERATE_RAINFOREST;
		}
	}
	else
	{
		if (Moisture < 0.33)
		{
			bytes = (uint8)DESERT;
		}
		else if ((Moisture >= 0.33) & (Moisture < 0.66))
		{
			bytes = (uint8)SAVANNA;
		}
		else
		{
			bytes = (uint8)RAIN_FOREST;
		}
	}
	return bytes;
}

void Generator::GenerateWithPerlinNoise(float** Map, EBiomType** BiomMap)
{
	float** TempHeightMap = new float*[MapSize];
	for (int i = 0; i < MapSize; i++)
	{
		TempHeightMap[i] = new float[MapSize];
	}
	USimplexNoiseBPLibrary::createSeed(WorldGenerationSettings->PerlinNoiseParameters.Sharp.Seed);
	USimplexNoiseBPLibrary::setNoiseSeed(WorldGenerationSettings->PerlinNoiseParameters.Sharp.Seed);
	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			const int XIndex = i + RightBorder;
			const int YIndex = j + RightBorder;
			float SharpNoise;
			float SmoothNoise;

			SharpNoise = USimplexNoiseBPLibrary::PerlinNoise2D_EX(
				i, -j, WorldGenerationSettings->PerlinNoiseParameters.Sharp.Lacunarity,
				WorldGenerationSettings->PerlinNoiseParameters.Sharp.Persistence,
				WorldGenerationSettings->PerlinNoiseParameters.Sharp.Octaves,
				WorldGenerationSettings->PerlinNoiseParameters.Sharp.NoiseDensity,
				WorldGenerationSettings->PerlinNoiseParameters.Sharp.ZeroToOne);


			SmoothNoise = USimplexNoiseBPLibrary::PerlinNoise2D_EX(
				i, -j, WorldGenerationSettings->PerlinNoiseParameters.Smooth.Lacunarity,
				WorldGenerationSettings->PerlinNoiseParameters.Smooth.Persistence,
				WorldGenerationSettings->PerlinNoiseParameters.Smooth.Octaves,
				WorldGenerationSettings->PerlinNoiseParameters.Smooth.NoiseDensity,
				WorldGenerationSettings->PerlinNoiseParameters.Smooth.ZeroToOne);

			SmoothNoise = Clamp(SmoothNoise, 0, 1);
			SharpNoise = Clamp(SharpNoise, 0, 1);

			TEnumAsByte<EBiomType> CurrentBiom;
			if (WorldGenerationSettings->PerlinNoiseParameters.IsTest)
			{
				CurrentBiom = BiomMap[XIndex][YIndex];
			}
			else
			{
				CurrentBiom = WorldGenerationSettings->PerlinNoiseParameters.DefaultBiom;
			}
			float FinalNoise = BezierComputationsInstance->FilterMap(SharpNoise, SmoothNoise, CurrentBiom);
			float Clamped = Clamp(FinalNoise, 0, 1);
			BezierComputationsInstance->CheckValue(Clamped, CurrentBiom);
			TempHeightMap[XIndex][YIndex] = Clamped;
		}

	if (WorldGenerationSettings->PerlinNoiseParameters.IsInvert)
	{
		InvertMap(TempHeightMap, BiomMap);
	}
	for (int i = 0; i < MapSize; i++)
	{
		for (int j = 0; j < MapSize; j++)
		{
			Map[i][j] = TempHeightMap[i][j];
		}
	}
	if (WorldGenerationSettings->IsApplyGausianFilter)
	{
		GausianFilterInstance->SmoothMap(TempHeightMap, Map);
	}
}

void Generator::InvertMap(float** MapForInvert, EBiomType** BiomMap)
{
	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			const int XIndex = i + RightBorder;
			const int YIndex = j + RightBorder;
			TEnumAsByte<EBiomType> CurrentBiom;
			if (WorldGenerationSettings->PerlinNoiseParameters.IsTest)
			{
				CurrentBiom = BiomMap[XIndex][YIndex];
			}
			else
			{
				CurrentBiom = WorldGenerationSettings->PerlinNoiseParameters.DefaultBiom;
			}
			FBiomData* CurrentBiomData = BezierComputationsInstance->DataSet.Find(CurrentBiom);
			float Noise = MapForInvert[XIndex][YIndex];
			Noise = CurrentBiomData->Max - (Noise - CurrentBiomData->Min);
			MapForInvert[XIndex][YIndex] = Noise;
		}
}

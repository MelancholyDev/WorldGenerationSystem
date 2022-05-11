#include "Classes/Generator.h"

#include "SimplexNoiseBPLibrary.h"

Generator::Generator(FGenerationParameters Parameters,UDataTable* Table)
{
	GausianParameters = Parameters.GausianParameters;
	PerlinNoiseParameters = Parameters.PerlinNoiseParameters;
	TemperatureParameters = Parameters.TemperatureAndMoistureParameters.TemperatureParameters;
	MoistureParameters = Parameters.TemperatureAndMoistureParameters.MoistureParameters;
	DiamondSquareParameters = Parameters.DiamondSquareParameters;
	GenerationParameters = Parameters;
	DataTableBiome = Table;
	InitializeBiomData();
	if (GenerationParameters.GenerationType == PERLIN_NOISE)
	{
		MapSize = (GenerationParameters.ChunkSize * 2 + 1) * PerlinNoiseParameters.Multiplier;
	}
	else
	{
		MapSize = FMath::Pow(2, GenerationParameters.DiamondSquareParameters.MapMultiplier) + 1;
	}
	LeftBorder = -(MapSize - 1) / 2;
	RightBorder = -LeftBorder;

	BezierComputationsInstance = new BezierComputations(BiomDataSet);
	DiamondSquareInstance = new DiamondSquare(DiamondSquareParameters, MapSize);
	GausianFilterInstance = new GausianFilter(GausianParameters, MapSize);
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

void Generator::GenerateHeightMap(float** Map, float** HeatMap, float** MoistureMap)
{
	switch (GenerationParameters.GenerationType)
	{
	case PERLIN_NOISE:
		{
			GenerateWithPerlinNoise(Map, HeatMap,MoistureMap);
		}break;
	case DIAMOND_SQUARE:
		{
			GenerateWithDiamondSquare(Map);
		}break;
	default: ;
	}
}

void Generator::GenerateBiomMaps(float** TemperatureMap,float** MoistureMap)
{
	USimplexNoiseBPLibrary::createSeed(TemperatureParameters.Seed);
	USimplexNoiseBPLibrary::setNoiseSeed(TemperatureParameters.Seed);
	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			float HeatNoise = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(
				i, -j, TemperatureParameters.Lacunarity, TemperatureParameters.Persistence,
				TemperatureParameters.Octaves, TemperatureParameters.NoiseDensity, TemperatureParameters.ZeroToOne);
			HeatNoise = Clamp(HeatNoise, 0, 1);
			const int XIndex = i + RightBorder;
			const int YIndex = j + RightBorder;

			TemperatureMap[XIndex][YIndex] = HeatNoise;
		}
	USimplexNoiseBPLibrary::createSeed(MoistureParameters.Seed);
	USimplexNoiseBPLibrary::setNoiseSeed(MoistureParameters.Seed);
	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			float MoistureNoise = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(
				i, -j, MoistureParameters.Lacunarity, MoistureParameters.Persistence,
				MoistureParameters.Octaves, MoistureParameters.NoiseDensity, MoistureParameters.ZeroToOne);
			MoistureNoise = Clamp(MoistureNoise, 0, 1);
			
			const int XIndex = i + RightBorder;
			const int YIndex = j + RightBorder;
			
			MoistureMap[XIndex][YIndex] = MoistureNoise;
		}
}


void Generator::GenerateSeaMap(float** Map)
{
}

void Generator::InitializeBiomData()
{
	TArray<FName> RowNames = DataTableBiome->GetRowNames();
	FString EmptyString;
	for (auto& Name : RowNames)
	{
		FBiomData Data = *DataTableBiome->FindRow<FBiomData>(Name, EmptyString);
		BiomDataSet.Add(Data.Type, Data);
	}
}
void Generator::GenerateWithDiamondSquare(float** Map)
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

uint8 Generator::GetBiom(float Heat,float Moisture)
{
	uint8 bytes;
	if (Heat < 0.33)
	{
		if(Moisture < 0.33)
		{
			bytes = (uint8)BOREAL;
		}else if ((Moisture >= 0.33) & (Moisture < 0.66))
		{
			bytes = (uint8)SEASONAL_FOREST;
		}else
		{
			bytes = (uint8)TUNDRA;
		}
	}
	else if ((Heat >= 0.33) & (Heat < 0.66))
	{
		if(Moisture < 0.33)
		{
			bytes = (uint8)SEASONAL_FOREST;
		}else if ((Moisture >= 0.33) & (Moisture < 0.66))
		{
			bytes = (uint8)TEMPERATE_FOREST;
		}else
		{
			bytes = (uint8)TEMPERATE_RAINFOREST;
		}
	}
	else
	{
		if(Moisture < 0.33)
		{
			bytes = (uint8)DESERT;
		}else if ((Moisture >= 0.33) & (Moisture < 0.66))
		{
			bytes = (uint8)SAVANNA;
		}else
		{
			bytes = (uint8)RAIN_FOREST;
		}
	}
	return bytes;
}

void Generator::GenerateWithPerlinNoise(float** Map, float** HeatMap,float** MoistureMap)
{
	
	float** TempHeightMap = new float*[MapSize];
	for (int i = 0; i < MapSize; i++)
	{
		TempHeightMap[i] = new float[MapSize];
	}
	USimplexNoiseBPLibrary::createSeed(PerlinNoiseParameters.Seed);
	USimplexNoiseBPLibrary::setNoiseSeed(PerlinNoiseParameters.Seed);
	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			const int XIndex = i + RightBorder;
			const int YIndex = j + RightBorder;
			float SharpNoise;
			float SmoothNoise;

			SharpNoise = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(
				i, -j, PerlinNoiseParameters.Lacunarity, PerlinNoiseParameters.Persistance,
				PerlinNoiseParameters.OctaveSharp,
				PerlinNoiseParameters.NoiseDensity, PerlinNoiseParameters.ZeroToOne);
			

			SmoothNoise = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(
				i, -j, PerlinNoiseParameters.Lacunarity, PerlinNoiseParameters.Persistance,
				PerlinNoiseParameters.OctaveSmooth,
				PerlinNoiseParameters.NoiseDensity, PerlinNoiseParameters.ZeroToOne);

			SmoothNoise = Clamp(SmoothNoise, 0, 1);
			SharpNoise = Clamp(SharpNoise, 0, 1);

			TEnumAsByte<EBiomType> CurrentBiom;
			if (PerlinNoiseParameters.IsTest)
			{
				CurrentBiom = (TEnumAsByte<EBiomType>)GetBiom(HeatMap[XIndex][YIndex],MoistureMap[XIndex][YIndex]);
			}
			else
			{
				CurrentBiom = PerlinNoiseParameters.Biom;
			}
			float FinalNoise = BezierComputationsInstance->FilterMap(SharpNoise, SmoothNoise, CurrentBiom);
			float Clamped = Clamp(FinalNoise, 0, 1);
			BezierComputationsInstance->CheckValue(Clamped, CurrentBiom);
			TempHeightMap[XIndex][YIndex] = Clamped;
		}

	if (PerlinNoiseParameters.IsInvert)
	{
		InvertMap(TempHeightMap, HeatMap,MoistureMap);
	}
	for(int i=0;i<MapSize;i++)
	{
		for(int j=0;j<MapSize;j++)
		{
			Map[i][j] = TempHeightMap[i][j];
		}
	}
	if (GenerationParameters.IsApplyGausianFilter)
	{
		GausianFilterInstance->SmoothMap(TempHeightMap, Map);
	}
}

void Generator::InvertMap(float** MapForInvert, float** HeatMap,float** MoistureMap)
{
	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			const int XIndex = i + RightBorder;
			const int YIndex = j + RightBorder;
			TEnumAsByte<EBiomType> CurrentBiom;
			if (PerlinNoiseParameters.IsTest)
			{
				CurrentBiom = (TEnumAsByte<EBiomType>)GetBiom(HeatMap[XIndex][YIndex],MoistureMap[XIndex][YIndex]);
			}
			else
			{
				CurrentBiom = PerlinNoiseParameters.Biom;
			}
			FBiomData* CurrentBiomData = BezierComputationsInstance->DataSet.Find(CurrentBiom);
			float Noise = MapForInvert[XIndex][YIndex];
			Noise = CurrentBiomData->Max - (Noise - CurrentBiomData->Min);
			MapForInvert[XIndex][YIndex] = Noise;
		}
}

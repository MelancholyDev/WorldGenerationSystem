#pragma once


UENUM()
enum EBiomType
{
	DESERT=0   UMETA(DisplayName = "Desert"),
	SAVANNA=1      UMETA(DisplayName = "Savanna"),
	TROPICAL_WOODLAND=2   UMETA(DisplayName = "TropicalWoodland"),
	TUNDRA=3   UMETA(DisplayName = "Tundra"),
	SEASONAL_FOREST=4   UMETA(DisplayName = "SeasonalForest"),
	RAIN_FOREST=5   UMETA(DisplayName = "RainForest"),
	TEMPERATE_FOREST=6   UMETA(DisplayName = "TemperateForest"),
	TEMPERATE_RAINFOREST=7   UMETA(DisplayName = "TemperateRainforest"),
	BOREAL=8   UMETA(DisplayName = "Boreal"),
};
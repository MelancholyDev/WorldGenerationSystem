#pragma once
#include "FWaterParams.generated.h"

USTRUCT()
struct FWaterParams
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float WaterLevel;
	UPROPERTY(EditAnywhere)
	bool IsAddRivers;
	UPROPERTY(EditAnywhere)
	int RiverWidth;
};

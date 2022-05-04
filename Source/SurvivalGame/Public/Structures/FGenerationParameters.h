#pragma once
#include "Enums/EGenerationType.h"
#include "FGenerationParameters.generated.h"

USTRUCT()
struct FGenerationParameters
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EGenerationType> GenerationType;
	UPROPERTY(EditAnywhere)
	int Baza;
};

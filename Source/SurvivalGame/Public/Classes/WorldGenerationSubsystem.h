// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WorldGenerationSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UWorldGenerationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void GenerateWorld();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Enums/EGenerationType.h"
#include "Structures/FDiamondSquareParameters.h"
#include "Structures/FGausianParameters.h"
#include "Structures/FPerlinNoiseParameters.h"
#include "WorldGenerationSettings.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UWorldGenerationSettings : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EGenerationType> GenerationType=EGenerationType::PERLIN_NOISE;

	//Width of world around you ATTENTION: Currently only uneven numbers are supported. If you write even number, chunk size will be *your number*+1 EXAMPLE: with RenderRange == 3, at one time 3*3 chunks will be in world
	UPROPERTY(EditAnywhere, Category="Chunk Settings")
	int RenderRange;
	//Width of one chunk ATTENTION: Currently only uneven numbers are supported. If you write even number, chunk size will be *your number*+1
	UPROPERTY(EditAnywhere, Category="Chunk Settings")
	int ChunkSize;
	//Size of one voxel in chunk
	UPROPERTY(EditAnywhere, Category="Chunk Settings")
	int VoxelSize;
	//If it is unchecked, your world will be static and not expand while player move
	UPROPERTY(EditAnywhere, Category="Chunk Settings")
	bool IsUpdateMap;
	//If checked Gausian Filter will be applied to height map
	UPROPERTY(EditAnywhere)
	bool IsApplyGausianFilter;

	UPROPERTY(EditAnywhere, meta=(EditCondition="GenerationType == EGenerationType::DIAMOND_SQUARE", EditConditionHides))
	FDiamondSquareParameters DiamondSquareParameters;
	UPROPERTY(EditAnywhere, meta=(EditCondition="GenerationType == EGenerationType::PERLIN_NOISE", EditConditionHides))
	FPerlinNoiseParameters PerlinNoiseParameters;
	UPROPERTY(EditAnywhere, meta=(EditCondition="IsApplyGausianFilter", EditConditionHides))
	FGausianParameters GausianParameters;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <vector>

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "SimplexNoise/Public/SimplexNoiseBPLibrary.h"
#include "VoxelChank.generated.h"

UCLASS()
class SURVIVALGAME_API AVoxelChank : public AActor
{
	GENERATED_BODY()

	float VoxelSize;
	int NoiseScale;
	int ChunkSize;
	int Depth;
	int MapSize;
	float** HeightMap;
	float** HeatMap;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstanceTopGrass;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstanceDirt;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstanceStone;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstanceSand;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstanceSnow;
	void ActorLocationVoxelWorldXY(const int XIndex, const int YIndex, int& X, int& Y) const;
	void ActorLocationVoxelWorldZ(const int ZIndex, int& Z) const;
	bool CheckInBound(int Index,int Size);

public:
	AVoxelChank();
	virtual void OnConstruction(const FTransform& Transform) override;
	void InitializeParameters(float VoxelSizeParam, int NoiseScaleParam, int ChunkSizeParam, int DepthParam,
	                          int MapSizeParam,float** MapParam,float** HeatParam);
};

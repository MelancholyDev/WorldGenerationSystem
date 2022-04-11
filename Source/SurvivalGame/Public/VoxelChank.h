// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "SimplexNoise/Public/SimplexNoiseBPLibrary.h"
#include "VoxelChank.generated.h"

UCLASS()
class SURVIVALGAME_API AVoxelChank : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	float NoiseDensity = 0.0001f;
	float VoxelSize = 100;
	int NoiseScale = 10;
	UPROPERTY(EditAnywhere)
	int ChunkSize = 5;
	UPROPERTY(EditAnywhere)
	int Depth = 10;
	UPROPERTY(VisibleAnywhere)
	USceneComponent * Root;
	float NoiseDensity3D = 0.001f;
	float Threshold3D = 0;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstanceTopGrass;
	UInstancedStaticMeshComponent* InstanceDirt;
	UInstancedStaticMeshComponent* InstanceStone;
	AVoxelChank();
private:
	void ActorLocationVoxelWorldXY(const int XIndex,const int YIndex,float& X,float& Y) const;
	void ActorLocationVoxelWorldZ(const int ZIndex, int& Z) const;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void InitializeParameters(float NoiseDensityParam,float VoxelSizeParam,int NoiseScaleParam,int ChunkSizeParam,int DepthParam,
		float NoiseDensity3DParam,float Threshold3DParam);
};

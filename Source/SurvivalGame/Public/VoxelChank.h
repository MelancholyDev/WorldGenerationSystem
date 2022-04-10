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
	float NoiseDesity = 0.0001f;
	float VoxelSize = 100;
	int NoiseScale = 10;
	UPROPERTY(EditAnywhere)
	int ChunkSize = 10;
	UPROPERTY(EditAnywhere)
	int Depth = 15;
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
	void ActorLocationVoxelWorld(const int XIndex,const int YIndex,const int ZIndex,float& X,float& Y,int& Z);
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

};

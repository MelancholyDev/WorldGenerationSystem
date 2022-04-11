// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "VoxelChank.h"
#include "GameFramework/PlayerController.h"
#include "Structures.h"
#include "Async/AsyncWork.h"
#include "GenerationPlayerController.generated.h"


UCLASS()
class SURVIVALGAME_API AGenerationPlayerController : public APlayerController
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	float ChunkSize;
	float ChunkLength;
	UPROPERTY(EditAnywhere)
	int RenderRange;
	UPROPERTY(EditAnywhere)
	int Depth;
	UPROPERTY(EditAnywhere)
	float NoiseDensity;
	UPROPERTY(EditAnywhere)
	float NoiseScale;
	UPROPERTY(EditAnywhere)
	float VoxelSize;
	UPROPERTY(EditAnywhere)
	float NoiseDensity3D;
	UPROPERTY(EditAnywhere)
	float Threshold3D;
	public:
	UPROPERTY(BlueprintReadWrite)
	FIntVector OldCoordinates;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AVoxelChank> ToSpawn;
	TArray<FVoxelLine>* Map;
	UFUNCTION(BlueprintCallable)
	FIntVector GetPlayerChunkCoordinates();
	UFUNCTION(BlueprintCallable)
	AVoxelChank* SpawnChunk(float X, float Y, float Z);
	void DeleteLine(int Index);
	void AddLine(bool IsForward);
	void AddColumn(bool isLeft);
	void AppendColumn(int Index,bool isLeft);
	void DeleteColumn(int Index);
	UFUNCTION(BlueprintCallable)
	void GetFullSize();
	UFUNCTION(BlueprintCallable)
	void XShift(int X);
	UFUNCTION(BlueprintCallable)
	void YShift(int Y);
	UFUNCTION(BlueprintCallable)
	void Diagonal(int X, int Y);
	FActorSpawnParameters* ChunkSpawnParameters;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};


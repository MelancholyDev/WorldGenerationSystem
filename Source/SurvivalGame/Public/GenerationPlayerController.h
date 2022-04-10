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

	UPROPERTY(EditAnywhere)
	int RenderRange;

	UPROPERTY(EditAnywhere)
	int Depth;
	
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
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};

class FAsyncSpawnChunkTask : public FNonAbandonableTask
{
private:
	float x;
	float y;
	float z;
public:
	FAsyncSpawnChunkTask(float x, float y, float z);
	void DoWork();
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAsyncSpawnChunkTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};

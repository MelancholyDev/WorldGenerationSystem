// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BT_EnemyController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SURVIVALGAME_API ABT_EnemyController : public AAIController
{
	GENERATED_BODY()

public:
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BehaviorTree;

	ABT_EnemyController();

	UPROPERTY(EditAnywhere,Blueprintable)
	class UPawnSensingComponent* PawnSensing;

	UFUNCTION()
		void OnSeePawn(APawn* PlayerPawn);

	UFUNCTION()
		void SetCanSeePlayer(bool SeePlayer, class UObject* Player);

	FTimerHandle RetriggerableTimerHandle;
	FTimerDelegate FunctionDelegate;
	void RunRetriggerableTimer();
};


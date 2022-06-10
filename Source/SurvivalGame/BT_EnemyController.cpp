// Fill out your copyright notice in the Description page of Project Settings.

#include "BT_EnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"


#include "Runtime/Engine/Public/EngineGlobals.h"


void ABT_EnemyController::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->OnSeePawn.AddDynamic(this, &ABT_EnemyController::OnSeePawn);
	RunBehaviorTree(BehaviorTree);
}
ABT_EnemyController::ABT_EnemyController()
{
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
}

void ABT_EnemyController::OnSeePawn(APawn* PlayerPawn)
{
	
	// ABT_EnemyController* Player = Cast<ABT_EnemyController>(PlayerPawn);
	//
	// if (Player)
	// {
	// 	SetCanSeePlayer(true, Player);
	// 	RunRetriggerableTimer();
	// }
	// else
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Green, TEXT("Simple message"));
	// }
}

void ABT_EnemyController::SetCanSeePlayer(bool SeePlayer, UObject* Player)
{
	// if (SeePlayer)
	// {
	// 	GetBlackboardComponent()
	// 		->SetValueAsBool(FName("ISAlert?"), SeePlayer);
	//
	// 	GetBlackboardComponent()
	// 		->SetValueAsObject(FName("PlayerTarget"), Player);
	// }
	// else
	// {
	// 	GetBlackboardComponent()
	// 		->SetValueAsBool(FName("ISAlert?"), SeePlayer);
	//
	// 	ACharacter* EnemyChar = Cast<ACharacter>(GetPawn());
	// 	EnemyChar->GetMesh()->GetAnimInstance()->StopAllMontages(0);
	// }
}


void ABT_EnemyController::RunRetriggerableTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(RetriggerableTimerHandle);

	FunctionDelegate.BindUFunction(this, FName("ISAlert?"),
		false, GetPawn());

	GetWorld()->GetTimerManager().SetTimer(RetriggerableTimerHandle,
		FunctionDelegate, PawnSensing->SensingInterval * 2.0f, false);
}
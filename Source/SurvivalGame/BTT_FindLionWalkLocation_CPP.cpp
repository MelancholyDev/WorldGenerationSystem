// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindLionWalkLocation_CPP.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTT_FindLionWalkLocation_CPP::
	ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	NavArea = FNavigationSystem::
		GetCurrent<UNavigationSystemV1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (NavArea)
	{
		NavArea->K2_GetRandomReachablePointInRadius(GetWorld(),
			GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(),
			RandomLocation, 5000.0f);
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	// inform blackboard about random location result
	OwnerComp.GetBlackboardComponent()
		->SetValueAsVector(FName("TargetLocation"), RandomLocation);

	return EBTNodeResult::Succeeded;
}
 
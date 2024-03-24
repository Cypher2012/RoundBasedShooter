// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBlueprintFunctionLibrary.h"

#include "Math/NumericLimits.h"
#include "Kismet/GameplayStatics.h"


void UGameBlueprintFunctionLibrary::SortActorsByDistanceToTarget(TArray<AActor*> Actors, FVector TargetLocation, AActor*& ClosestActor)
{	
	ClosestActor = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();

	for (AActor* FoundActor : Actors)
	{
		FVector DistanceVector = FoundActor->GetActorLocation() - TargetLocation;
		float DistanceToTarget = DistanceVector.Size();

		if (DistanceToTarget < ClosestDistance)
		{
			ClosestDistance = DistanceToTarget;
			ClosestActor = FoundActor;
		}
	}

	return;
}




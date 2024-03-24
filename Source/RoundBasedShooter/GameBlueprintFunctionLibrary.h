// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"


#include "GameBlueprintFunctionLibrary.generated.h"

UCLASS()
class ROUNDBASEDSHOOTER_API UGameBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	/**
		Finds the actor that is closest to the target location. Does not change the order of the input array of actors!
		@param Actors - The TArray of actors that will be checked
		@param TargetLocation - Find the actor closest to this target location
		@param MaxDistance - The starting distance for the sort algorithm. Strongly advise this to be a high number!
		@param ClosestActor - Returned actor that is closest to the target location
	*/
	UFUNCTION(BlueprintCallable, Category = "Sorting")
	static void SortActorsByDistanceToTarget(const TArray<AActor*> Actors,const FVector TargetLocation, AActor* &ClosestActor);

};

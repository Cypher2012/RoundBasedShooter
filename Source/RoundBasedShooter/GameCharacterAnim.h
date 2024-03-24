// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "GameCharacterAnim.generated.h"

class UAnimSequence;

UCLASS()
class ROUNDBASEDSHOOTER_API UGameCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UGameCharacterAnim();

	// the base pose used for the blueprint. This can be overriten by individual inventory items
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimSequence* IdleAnimation;

	// The new idle animation pose to use on the character
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Animations")
	void UpdateIdleAnimation(UAnimSequence* NewIdleAnim);

protected:


};

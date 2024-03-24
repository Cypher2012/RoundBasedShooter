// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameCharacterBase.generated.h"

UCLASS()
class ROUNDBASEDSHOOTER_API AGameCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AGameCharacterBase();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// If the character is alive or not
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Character");
	bool bIsAlive;

protected:

	virtual void BeginPlay() override;

private:
		

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiniMapGenerator.generated.h"

class USceneCaptureComponent;
class UTextureRenderTarget2D;

UCLASS()
class ROUNDBASEDSHOOTER_API AMiniMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AMiniMapGenerator();

protected:

	virtual void BeginPlay() override;

	// Scene capture component used to capture the scene
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mini Map")
	USceneCaptureComponent2D* MiniMapCaptureComponent;

private:


};

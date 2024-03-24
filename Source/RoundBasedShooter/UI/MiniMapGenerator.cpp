// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapGenerator.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

AMiniMapGenerator::AMiniMapGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	MiniMapCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>("MiniMapCaptureComponent");
	MiniMapCaptureComponent->SetRelativeLocationAndRotation(FVector(0, 0, 5000), FRotator(-90.0f, 0, 0));
	MiniMapCaptureComponent->ProjectionType = ECameraProjectionMode::Orthographic;
	MiniMapCaptureComponent->OrthoWidth = 5000;
	MiniMapCaptureComponent->CaptureSource = ESceneCaptureSource::SCS_BaseColor;

	MiniMapCaptureComponent->SetupAttachment(RootComponent);
}

void AMiniMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}




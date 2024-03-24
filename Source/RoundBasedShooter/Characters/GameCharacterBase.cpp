
#include "GameCharacterBase.h"


AGameCharacterBase::AGameCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsAlive = true;
}

void AGameCharacterBase::BeginPlay()
{
	Super::BeginPlay();	
}

void AGameCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


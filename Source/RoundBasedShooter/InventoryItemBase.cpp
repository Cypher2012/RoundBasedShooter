// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemBase.h"

#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GameCharacterAnim.h"


void AInventoryItemBase::DepleteRounds(int NumRounds)
{
	
	ItemAmmoInfo.NumRounds = FMath::Clamp<int>(ItemAmmoInfo.NumRounds - NumRounds, 0, INT_MAX);
}

void AInventoryItemBase::ConsumeMagazine(bool InfiniteAmmo)
{
	 
	if (InfiniteAmmo)
	{
		ItemAmmoInfo.NumRounds = ItemAmmoInfo.MaxRounds;
	}

	else if (ItemAmmoInfo.NumMagazines > 0)
	{
		ItemAmmoInfo.NumMagazines = ItemAmmoInfo.NumMagazines - 1;
		ItemAmmoInfo.NumRounds = ItemAmmoInfo.MaxRounds;
	}	
}

bool AInventoryItemBase::AvailableRounds() const
{
	
	if (ItemAmmoInfo.NumRounds > 0)
	{
		return true;
	}
	
	return false;
}

bool AInventoryItemBase::GetIsEquipped() const
{
	return IsEquipped;
}

UAnimSequence* AInventoryItemBase::GetCharacterEquipAnim() const
{
	return ItemAnimations.Character_EquipAnim;
}

UAnimSequence* AInventoryItemBase::GetItemEquipAnim() const
{
	return ItemAnimations.Item_EquipAnim;
}

// Sets default values
AInventoryItemBase::AInventoryItemBase()
{
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EquipSocketName = "S_GripPoint";
	IsEquipped = false;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ItemMesh");
	if (ItemMesh)
	{
		RootComponent = ItemMesh;
	}

}

void AInventoryItemBase::UpdateIdleAnimation(UInventoryComponentBase* InventoryComponent)
{	
	if (InventoryComponent)
	{
		ACharacter* OwningCharacter = Cast<ACharacter>(InventoryComponent->GetOwner());
		if (OwningCharacter)
		{
			USkeletalMeshComponent* AnimatedMesh = OwningCharacter->GetMesh();
			if (AnimatedMesh)
			{
				UGameCharacterAnim* CharacterAnim = Cast<UGameCharacterAnim>(AnimatedMesh->GetAnimInstance());
				if (CharacterAnim)
				{
					CharacterAnim->UpdateIdleAnimation(ItemAnimations.Character_IdleAnim);
				}
			}
		}
	}	
}

void AInventoryItemBase::OnRequestReloadEvent_Implementation(bool InfiniteAmmo)
{
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, __FUNCTION__);
}

void AInventoryItemBase::OnReplenish_Implementation()
{
	
	ItemAmmoInfo.NumMagazines = ItemAmmoInfo.MaxMagazines;
	ItemAmmoInfo.NumRounds = ItemAmmoInfo.MaxRounds;
}

void AInventoryItemBase::OnEquip_Implementation(UInventoryComponentBase* InventoryComponent)
{
	
	if (!InventoryComponent)
	{
		return;
	}

	StoredInventoryComponent = InventoryComponent;	
	UpdateIdleAnimation(InventoryComponent);
	IsEquipped = true;	
}

void AInventoryItemBase::OnUnEquip_Implementation()
{	
	IsEquipped = false;
	if (IsValid(RootComponent))
	{
		RootComponent->SetVisibility(false, true);
	}	
}

void AInventoryItemBase::OnFirePressed_Implementation()
{
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "OnFirePressed");
}

void AInventoryItemBase::OnFireReleased_Implementation()
{
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "OnFireReleased");
}

void AInventoryItemBase::OnCancelReloadEvent_Implementation()
{
	
}

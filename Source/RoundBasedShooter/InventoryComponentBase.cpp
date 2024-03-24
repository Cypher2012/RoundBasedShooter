// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponentBase.h"

#include "InventoryItemBase.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UInventoryComponentBase::UInventoryComponentBase()
{
	PrimaryComponentTick.bCanEverTick = false;

	LoadoutActors.AddDefaulted(5);

	LastEquippedWeapon = ESlotOption::PrimaryMainWeapon;
	LastEquippedGadget = ESlotOption::PrimaryGadget;

}

void UInventoryComponentBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DestroyItems();
}

bool UInventoryComponentBase::AddItem(ESlotOption SlotOption, TSubclassOf<AInventoryItemBase> ItemClass)
{

	if (!ItemClass)
	{
		return false;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = GetOwner();

	AInventoryItemBase* SpawnedItem = GetWorld()->SpawnActor<AInventoryItemBase>(ItemClass, SpawnParams);

	int SlotOptionIndex = SlotOption;
	AInventoryItemBase* InvItem = GetLoadoutActor(SlotOptionIndex);

	if (InvItem)
	{
		InvItem->Destroy();
	}

	if (LoadoutActors.IsValidIndex(SlotOptionIndex))
	{
		LoadoutActors[SlotOptionIndex] = SpawnedItem;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "AddItem: SlotOptionIndex invalid index!");
	}
	

	return true;
}

void UInventoryComponentBase::DestroyItems()
{
	UnEquipAll();

	for (int i = LoadoutActors.Num() - 1; i >= 0; i--)
	{
		if (IsValid(GetLoadoutActor(i)))
		{
			GetLoadoutActor(i)->Destroy();
		}
	}
}

void UInventoryComponentBase::UnEquipAll()
{
	for (AInventoryItemBase* InvItem : LoadoutActors)
	{
		if (IsValid(InvItem))
		{
			InvItem->OnUnEquip();
		}		
	}
}

AInventoryItemBase* UInventoryComponentBase::GetLoadoutActor(int ActorIndex) const
{
	if (!LoadoutActors.IsValidIndex(ActorIndex))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "GetLoadoutActor InvalidIndex!");
		return nullptr;
	}
	else
	{
		return LoadoutActors[ActorIndex];
	}
}

TArray<AInventoryItemBase*> UInventoryComponentBase::GetLoadoutActors() const
{
	return LoadoutActors;
}

TEnumAsByte<ESlotOption> UInventoryComponentBase::GetEquippedSlot() const
{
	return CurrentEquippedSlot;
}

ESlotOption UInventoryComponentBase::PickEmptyWeaponSlot() const
{
	AInventoryItemBase* TempItem = GetLoadoutActor(ESlotOption::PrimaryMainWeapon);

	// If primary slot is empty, return primary weapon slot	
	if (!TempItem)
	{
		return ESlotOption::PrimaryMainWeapon;
	}	

	// If secondary slot is empty, return secondary weapon slot
	TempItem = GetLoadoutActor(ESlotOption::SecondaryMainWeapon);
	if (!TempItem)
	{
		return ESlotOption::SecondaryMainWeapon;
	}

	// If both slots have items, then return the last equipped weapon
	else
	{
		return LastEquippedWeapon;
	}
}

ESlotOption UInventoryComponentBase::PickEmptyGadgetSlot() const
{
	// If primary slot is empty, return primary gadget slot
	if (!GetLoadoutActor(ESlotOption::PrimaryGadget))
	{
		return ESlotOption::PrimaryGadget;
	}

	// If secondary slot is empty, return secondary gadget slot
	if (!GetLoadoutActor(ESlotOption::SecondaryGadget))
	{
		return ESlotOption::SecondaryGadget;
	}

	// If both slots have items, then return the last equipped gadget
	else
	{
		return LastEquippedGadget;
	}
}

bool UInventoryComponentBase::IsItemInInventory(TSubclassOf<AInventoryItemBase> CheckClass) const
{
	if (!CheckClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "IsItemInInventory : CheckClass = nullptr");
		return false;
	}

	for (AInventoryItemBase* ItemActor : LoadoutActors)
	{
		if (ItemActor)
		{
			if (ItemActor->GetClass() == CheckClass)
			{
				return true;
			}
		}		
	}

	return false;
}

bool UInventoryComponentBase::PlayEquipAnimation(AInventoryItemBase* CurrentItem, FName SlotName)
{
	if (IsValid(CurrentItem))
	{
		UAnimSequence* EquipAnim = CurrentItem->GetCharacterEquipAnim();
		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

		if (IsValid(OwnerCharacter) && IsValid(EquipAnim))
		{
			UAnimInstance* OwnerAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
			if (IsValid(OwnerAnimInstance))
			{
				OwnerAnimInstance->PlaySlotAnimationAsDynamicMontage(EquipAnim, SlotName);
				return true;
			}
		}
	}

	return false;
}

void UInventoryComponentBase::SendFirePressed(TEnumAsByte<ESlotOption> SlotOption)
{
	AInventoryItemBase* SelectedItem = GetLoadoutActor(SlotOption);
	if (IsValid(SelectedItem))
	{
		SelectedItem->OnFirePressed();
	}
}

void UInventoryComponentBase::SendFireReleased(TEnumAsByte<ESlotOption> SlotOption)
{
	AInventoryItemBase* SelectedItem = GetLoadoutActor(SlotOption);
	if (IsValid(SelectedItem))
	{
		SelectedItem->OnFireReleased();
	}
}

void UInventoryComponentBase::EquipItem(TEnumAsByte<ESlotOption> SlotOption, FName SlotName)
{
	// Cancel operation if the item is the same as current item and is equipped OR check if the item trying to be equipped is invalid
	if (SlotOption == CurrentEquippedSlot && GetLoadoutActor(CurrentEquippedSlot)->GetIsEquipped() || !IsValid(GetLoadoutActor(SlotOption)))
	{
		return;
	}

	CurrentEquippedSlot = SlotOption;
	CancelReload();
	UnEquipAll();

	// Set last equipped depending on whether the newly equipped item is a weapon or a gadget
	if (CurrentEquippedSlot == ESlotOption::PrimaryMainWeapon || CurrentEquippedSlot == ESlotOption::SecondaryMainWeapon)
	{
		LastEquippedWeapon = CurrentEquippedSlot;
	}
	else if (CurrentEquippedSlot == ESlotOption::PrimaryGadget || CurrentEquippedSlot == ESlotOption::SecondaryGadget)
	{
		LastEquippedGadget = CurrentEquippedSlot;
	}
	
	// Go ahead and actually equip the item
	AInventoryItemBase* CurrentItem = GetLoadoutActor(CurrentEquippedSlot);

	if (IsValid(CurrentItem))
	{
		CurrentItem->OnEquip(this);
		PlayEquipAnimation(CurrentItem, SlotName);
	}
}

bool UInventoryComponentBase::SwapItem(TSubclassOf<AInventoryItemBase> NewItemClass, bool bShouldEquip)
{
	// Return false if the item is already in the inventory or the passed in item class is nullptr
	if (!NewItemClass || IsItemInInventory(NewItemClass))
	{
		return false;
	}

	ESlotOption NewSlotOption;
	AInventoryItemBase* DefaultItem = Cast<AInventoryItemBase>(NewItemClass->GetDefaultObject(true));
	
	// Choose which slot the new item will go in to
	if (DefaultItem->InventorySlotType == ESlotType::WeaponType)
	{
		NewSlotOption = PickEmptyWeaponSlot();
	}
	else if (DefaultItem->InventorySlotType == ESlotType::GadgetType)
	{
		NewSlotOption = PickEmptyGadgetSlot();
	}
	else
	{
		NewSlotOption = ESlotOption::Throwable;
	}

	AddItem(NewSlotOption, NewItemClass);

	if (bShouldEquip && NewSlotOption != ESlotOption::Throwable)
	{
		EquipItem(NewSlotOption, "UpperBodySlot");
	}
	else
	{
		EquipItem(CurrentEquippedSlot, "UpperBodySlot");
	}

	return true;
}

void UInventoryComponentBase::ReplenishAllAmmo()
{
	for (AInventoryItemBase* ItemActor : LoadoutActors)
	{
		if (IsValid(ItemActor))
		{
			ItemActor->OnReplenish();
		}
	}
}

void UInventoryComponentBase::OnFirePressed()
{
	SendFirePressed(CurrentEquippedSlot);
}

void UInventoryComponentBase::OnFireReleased()
{
	SendFireReleased(CurrentEquippedSlot);
}

void UInventoryComponentBase::OnThrowPressed()
{
	SendFirePressed(Throwable);
}

void UInventoryComponentBase::OnThrowReleased()
{
	SendFireReleased(Throwable);
}

void UInventoryComponentBase::ReloadSelected()
{
	AInventoryItemBase* SelectedItem = GetLoadoutActor(CurrentEquippedSlot);
	if (IsValid(SelectedItem))
	{
		SelectedItem->OnRequestReloadEvent(bInfiniteAmmo);
	}
}

void UInventoryComponentBase::CancelReload()
{
	// Loop through all loadout items and call their OnCancelReloadEvent method
	for (AInventoryItemBase* ItemActor : LoadoutActors)
	{
		if (IsValid(ItemActor))
		{
			ItemActor->OnCancelReloadEvent();
		}		
	}
}

AInventoryItemBase* UInventoryComponentBase::GetSelectedItem() const
{
	return GetLoadoutActor(CurrentEquippedSlot);
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponentBase.generated.h"

class AInventoryItemBase;

UENUM(Blueprintable)
enum ESlotOption
{
	PrimaryMainWeapon UMETA(DisplayName = "PrimaryMainWeapon"),
	SecondaryMainWeapon UMETA(DisplayName = "SecondaryMainWeapon"),
	Throwable UMETA(DisplayName = "Throwable"),
	PrimaryGadget UMETA(DisplayName = "PrimaryGadget"),
	SecondaryGadget UMETA(DisplayName = "SecondaryGadget")
};

UENUM(Blueprintable)
enum ESlotType
{
	WeaponType UMETA(DisplayName = "Weapon"),
	GadgetType UMETA(DisplayName = "Gadget"),
	ThrowableType UMETA(DisplayName = "Throwable")
};


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROUNDBASEDSHOOTER_API UInventoryComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	

	UFUNCTION(BlueprintPure, Category = "Loadout")
	TArray<AInventoryItemBase*> GetLoadoutActors() const;

	// Get the current equipped slot
	UFUNCTION(Blueprintpure, Category = "Loadout")
	TEnumAsByte<ESlotOption> GetEquippedSlot() const;

	UInventoryComponentBase();

	// Equip the item in the given SlotOption. 
	// @param SlotOption - The slot to equip
	// @param SlotName - The name of the slot used in the animation blueprint to play the equip animation on
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void EquipItem(TEnumAsByte<ESlotOption> SlotOption, FName SlotName);

	// Trys to swap item with the new item class. Will not swap if already in inventory
	// @param NewItemClass - New item to be swapped
	// @param ShouldEquip - If the item should be equipped after the swap is complete
	UFUNCTION (BlueprintCallable, Category = "Loadout")
	bool SwapItem(TSubclassOf<AInventoryItemBase> NewItemClass, bool bShouldEquip = false);
		
	// Goes through all items and calls their OnReplenish event
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void ReplenishAllAmmo();

	// Called when character calls OnFirePressed. Inventory component will then trigger the OnFirePressed method on the selected item
	UFUNCTION(BlueprintCallable, Category = "Firing")
	void OnFirePressed();

	// Called when character calls OnFireReleased. Inventory component will then trigger the OnFireReleased method on the selected item
	UFUNCTION(BlueprintCallable, Category = "Firing")
	void OnFireReleased();

	// Called when the character calls the OnThrowPressed function. Inventory component will call the OnFirePressed function on the throwable
	UFUNCTION(BlueprintCallable, Category = "Throwing")
	void OnThrowPressed();

	// Called when the character calls the OnThrowReleased function. Inventory component will call the OnFireReleased function on the throwable
	UFUNCTION(BlueprintCallable, Category = "Throwing")
	void OnThrowReleased();

	// Called when character presses OnReload. Calls OnRequestReload event on the selected item
	UFUNCTION(BlueprintCallable, Category = "Reloading")
	void ReloadSelected();

	// Called when we need to cancel the reload. You might call this when swapping weapons for example.
	UFUNCTION(BlueprintCallable, Category = "Reloading")
	void CancelReload();

	// Returns the currently selected item
	UFUNCTION(BlueprintPure, Category = "Loadout")
	AInventoryItemBase* GetSelectedItem() const;

protected:

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
		Adds the passed item to the inventory into the chosen slot.
		Returns false if the item could not be added. Returns true if the item was added.

		@param SlotOption - Where in the inventory this item will be added
		@param ItemClass - The class of the item that is being added
	*/
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	bool AddItem(ESlotOption SlotOption, TSubclassOf<AInventoryItemBase> ItemClass);

	// Check if the given class is already in the inventory
	UFUNCTION(BlueprintPure, Category = "Loadout")
	bool IsItemInInventory(TSubclassOf<AInventoryItemBase> CheckClass) const;

	// If this inventory can have infinite ammo
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ammo")
	bool bInfiniteAmmo;

private:

	// Plays the equip animation using the given animation slot
	bool PlayEquipAnimation(AInventoryItemBase* CurrentItem, FName SlotName);

	// Calls the OnFirePressed function on the item passed
	void SendFirePressed(TEnumAsByte<ESlotOption> SlotOption);

	// Calls the OnFireReleased function on the item passed
	void SendFireReleased(TEnumAsByte<ESlotOption> SlotOption);

	// Tries to pick the best weapon slot to swap a new weapon in to. Returns the current selected weapon slot if neither is empty
	ESlotOption PickEmptyWeaponSlot() const;

	// Tries to pick the best slot to swap a new gadget in to. Returns the last selected gadget slot if neither is empty
	ESlotOption PickEmptyGadgetSlot() const;

	// Calls UnEquipAll and then destroys all the item actors
	void DestroyItems();

	// Goes through all the items and calls their UnEquip method
	void UnEquipAll();

	AInventoryItemBase* GetLoadoutActor(int ActorIndex) const;

	TEnumAsByte<ESlotOption> CurrentEquippedSlot;
	TEnumAsByte<ESlotOption> LastEquippedWeapon;
	TEnumAsByte<ESlotOption> LastEquippedGadget;
	TArray<AInventoryItemBase*> LoadoutActors;
};

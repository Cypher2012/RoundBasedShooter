// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryComponentBase.h"
#include "Animation/AnimSequence.h"
#include "Sound/SoundWave.h"

#include "InventoryItemBase.generated.h"

class USceneComponent;
class USkeletalMeshComponent;


USTRUCT(BlueprintType)
struct FAmmoInfo
{

public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int MaxMagazines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int NumMagazines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int MaxRounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int NumRounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	bool UsesMagazines;

	FAmmoInfo()
	{
		NumMagazines = MaxMagazines = 10;
		NumRounds = MaxRounds = 10;
		UsesMagazines = true;
	}
};

USTRUCT(BlueprintType)
struct FAnimationData
{
	GENERATED_BODY()

public:

	// animation to use on the character when equipping the item
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Character_EquipAnim;

	// Animation to use on the item when equipping the item
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Item_EquipAnim;

	// Base idle animation to use on the character
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Character_IdleAnim;

	// Base idle animation to use on the item
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Item_IdleAnim;

	// Animation to play on the character when reloading weapon
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Character_OnReloadAnim;

	// Animation to play on the item on reload
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Item_OnReloadAnim;

	// Animation to play on the character on fire pressed 
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Character_OnFirePressedAnim;

	// Animation to play on the item when on fire pressed
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Item_OnFirePressedAnim;

	// Animation to play on the character on fire released
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Character_OnFireReleasedAnim;

	// Animation to play on the item on fire released
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Item_OnFireReleasedAnim;

	// Animation to play on the character on throw
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Character_ThrowAnim;

	// Animation to play on the item on throw
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Item_ThrowAnim;

	FAnimationData()
	{
		Character_EquipAnim = nullptr;
		Item_EquipAnim = nullptr;
		Character_IdleAnim = nullptr;
		Item_IdleAnim = nullptr;
		Character_OnReloadAnim = nullptr;
		Item_OnReloadAnim = nullptr;
		Character_OnFirePressedAnim = nullptr;
		Item_OnFirePressedAnim = nullptr;
		Character_OnFireReleasedAnim = nullptr;
		Character_OnFireReleasedAnim = nullptr;
		Item_OnFireReleasedAnim = nullptr;
		Character_ThrowAnim = nullptr;
		Item_ThrowAnim = nullptr;
	}
};

USTRUCT(BlueprintType)
struct FSoundData
{
	GENERATED_BODY()

public:

	// Sound played when OnFirePressed triggered
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Audio")
	USoundWave* OnFirePressedSound;

	// Sound played on reload
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Audio")
	USoundWave* OnReloadSound;

	// Sound played on equip
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Audio")
	USoundWave* OnEquipSound;

	// Sound played on unequip
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Audio")
	USoundWave* OnUnEquipSound;

	// On empty magazine 
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Audio")
	USoundWave* EmptyMagazineSound;

	FSoundData()
	{
		OnFirePressedSound = nullptr;
		OnReloadSound = nullptr;
		OnEquipSound = nullptr;
		OnUnEquipSound = nullptr;
		EmptyMagazineSound = nullptr;
	}
};

UCLASS()
class ROUNDBASEDSHOOTER_API AInventoryItemBase : public AActor
{
	GENERATED_BODY()
	
private:

	void UpdateIdleAnimation(UInventoryComponentBase* InventoryComponent);

protected:


	// The name of the socket that items will attach to
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FName EquipSocketName;

	// Flag to check if this item is currently equipped
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	bool IsEquipped;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	UInventoryComponentBase* StoredInventoryComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	USkeletalMeshComponent* ItemMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FAmmoInfo ItemAmmoInfo;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void DepleteRounds(int NumRounds = 1);

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void ConsumeMagazine(bool InfiniteAmmo = false);

	UFUNCTION(BlueprintPure, Category = "Ammo")
	bool AvailableRounds() const;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Animation")
	FAnimationData ItemAnimations;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Audio")
	FSoundData ItemSounds;

public:	

	// Get if the item is equipped
	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool GetIsEquipped() const;

	// Returns the character equip animation sequence
	UFUNCTION(BlueprintPure, Category = "Animation")
	UAnimSequence* GetCharacterEquipAnim() const;

	// Returns the item equip animation sequence
	UFUNCTION(BlueprintPure, Category = "Animation")
	UAnimSequence* GetItemEquipAnim() const;

	// The type of inventory slot this goes in
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TEnumAsByte<ESlotType> InventorySlotType;

	// Sets default values for this actor's properties
	AInventoryItemBase();

	// Called when the fire input is pressed
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ammo")
	void OnReplenish();

	// Called when the fire input is pressed
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firing")
	void OnFirePressed();

	// Called when the fire input is released
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firing")
	void OnFireReleased();
	
	// Called when the item is equipped
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipping")
	void OnEquip(UInventoryComponentBase* InventoryComponent);

	// Called when the item is unequipped
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipping")
	void OnUnEquip();

	// Called when a request to reload is fired
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Reloading")
	void OnRequestReloadEvent(bool InfiniteAmmo = false);

	// Called to request a cancel of the reload. Likely due to the weapon being switched for example
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Reloading")
	void OnCancelReloadEvent();
};

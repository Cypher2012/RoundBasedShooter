

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

class ASpawnPoint;

UENUM(Blueprintable)
enum ERoundState
{
	InRound UMETA(DisplayName = "InRound"),
	Cooldown UMETA(DisplayName = "Cooldown")
};

UCLASS()
class ROUNDBASEDSHOOTER_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	

	ASpawnManager();

	virtual void Tick(float DeltaTime) override;

	// The current game round
	UFUNCTION(BlueprintPure, Category = "Spawning")
	int GetCurrentRound() const;

protected:

	virtual void BeginPlay() override;

	// The spawn point blueprint class
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<ASpawnPoint> SpawnPointClass;

	// Array containing all the classes of basic enemies that can be spawned
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawning")
	TArray<TSubclassOf<AActor>> BasicEnemyClassArray;

	// Array containing all the classes of harder enemies that can be spawned
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawning")
	TArray<TSubclassOf<AActor>> HardEnemyClassArray;

	// Spawns an enemy
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnEnemy(bool bSpawnHardEnemy = false);

	UFUNCTION(BlueprintPure, Category = "Spawning")
	int GetNumRemainingEnemies() const;

	UPROPERTY(BlueprintReadWrite, Category = "Spawning")
	bool bIsSpawning;

	UFUNCTION(BlueprintPure, Category = "Spawning")
	ASpawnPoint* GetRandomSpawnPoint() const;

	// Current round state
	UPROPERTY(BlueprintReadWrite, Category = "Spawning")
	TEnumAsByte<ERoundState> CurrentRoundState;

	// Amount of cool down time between rounds
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawning")
	float CooldownTime;

	// Max number of enemies that can be alive at one time. Used to prevent spawning excessive amount of enemy actors
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawning")
	int MaxEnemies;

	// Number of enemies spawned for this round. We keep track of this in case player 
	// manages to kill all enemies before we finish spawning them all and so to prevent a false round end
	UPROPERTY(BlueprintReadWrite, Category = "Spawning")
	int NumEnemiesSpawned;

	// The amount of time to elapse between spawning enemies. 
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawning")
	float EnemySpawnDelay;

	// Used to multiply the current round to get the number of enemies to spawn this round
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawning")
	int SpawnMultiplier;

	// Increment the current round by 1
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void IncrementCurrentRound();

	// Destroys all enemy actors. Dead or alive. Used to clean up enemies before new round start
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void CleanupEnemies();

private:

	// Gets and saves all spawn points in the level
	TArray<ASpawnPoint*> FindSpawnPoints();

	TArray<ASpawnPoint*> SpawnPoints;		

	TSubclassOf<AActor> GetRandomBasicEnemyClass() const;
	TSubclassOf<AActor> GetRandomHardEnemyClass() const;

	int CurrentRound;

	// Loops through the array of basic and hard actor classes and runs GetAllActorsofClass. WARNING: Expensive. Do not use on tick
	TArray<AActor*> GetAllEnemyActors() const;
	
};

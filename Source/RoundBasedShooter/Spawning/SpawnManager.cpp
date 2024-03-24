
#include "SpawnManager.h"

#include "SpawnPoint.h"
#include "../Characters/GameCharacterBase.h"

#include "Kismet/GameplayStatics.h"

ASpawnManager::ASpawnManager()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentRoundState = ERoundState::Cooldown;
	CooldownTime = 4.0f;
	MaxEnemies = 20;
	NumEnemiesSpawned = 0;
	EnemySpawnDelay = 1.0f;
	SpawnMultiplier = 5;
	CurrentRound = 0;

}

void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();

	FindSpawnPoints();
	
}

AActor* ASpawnManager::SpawnEnemy(bool bSpawnHardEnemy)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	ASpawnPoint* SpawnPoint = GetRandomSpawnPoint();

	AActor* SpawnedActor = nullptr;

	if (!bSpawnHardEnemy)
	{
		SpawnedActor = GetWorld()->SpawnActor<AActor>(GetRandomBasicEnemyClass(), SpawnPoint->GetActorTransform(), SpawnParams);
	}
	else
	{
		SpawnedActor = GetWorld()->SpawnActor<AActor>(GetRandomHardEnemyClass(), SpawnPoint->GetActorTransform(), SpawnParams);
	}
	

	return SpawnedActor;
}

int ASpawnManager::GetNumRemainingEnemies() const
{
	int NumRemainingEnemies = 0;

	for (AActor* IActor : GetAllEnemyActors())
	{
		AGameCharacterBase* GameCharacter = Cast<AGameCharacterBase>(IActor);
		if (IsValid(GameCharacter) && GameCharacter->bIsAlive)
		{
			NumRemainingEnemies++;
		}
	}

	return NumRemainingEnemies;
}

void ASpawnManager::IncrementCurrentRound()
{
	CurrentRound++;
}

void ASpawnManager::CleanupEnemies()
{
	for (AActor* IActor : GetAllEnemyActors())
	{
		IActor->Destroy();
	}
}

TArray<ASpawnPoint*> ASpawnManager::FindSpawnPoints()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnPointClass, FoundActors);

	for (AActor* SpawnPointActor : FoundActors)
	{
		ASpawnPoint* SpawnPoint = Cast<ASpawnPoint>(SpawnPointActor);
		if (SpawnPoint)
		{
			SpawnPoints.Add(SpawnPoint);
		}
	}

	return SpawnPoints;
}

TSubclassOf<AActor> ASpawnManager::GetRandomBasicEnemyClass() const
{
	int ClassIndex = FMath::FRandRange(0, BasicEnemyClassArray.Num() - 1);

	if (BasicEnemyClassArray.IsValidIndex(ClassIndex))
	{
		return BasicEnemyClassArray[ClassIndex];
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "GetRandomBasicEnemyClass: ClassIndex invalid!");
		return nullptr;
	}
}

TSubclassOf<AActor> ASpawnManager::GetRandomHardEnemyClass() const
{
	int ClassIndex = FMath::FRandRange(0, HardEnemyClassArray.Num() - 1);

	if (HardEnemyClassArray.IsValidIndex(ClassIndex))
	{
		return HardEnemyClassArray[ClassIndex];
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "GetRandomHardEnemyClass: ClassIndex invalid!");
		return nullptr;
	}
}

TArray<AActor*> ASpawnManager::GetAllEnemyActors() const
{
	TArray<AActor*> CombinedFoundActors;

	// Get all actors of classes found in BasicEnemyClassArray
	for (TSubclassOf<AActor> ActorClass : BasicEnemyClassArray)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ActorClass, FoundActors);
				
		for (AActor* IActor : FoundActors)
		{
			CombinedFoundActors.Add(IActor);
		}
	}

	// Get all actors of classes found in HardEnemyClassArray
	for (TSubclassOf<AActor> ActorClass : HardEnemyClassArray)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ActorClass, FoundActors);

		for (AActor* IActor : FoundActors)
		{
			CombinedFoundActors.Add(IActor);
		}
	}

	return CombinedFoundActors;
}

ASpawnPoint* ASpawnManager::GetRandomSpawnPoint() const
{
	if (SpawnPoints.Num() <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "GetRandomSpawnPoint: There were no spawn points found!");
		return nullptr;
	}

	int NumSpawnPoints = SpawnPoints.Num();

	int SpawnPointIndex = FMath::FRandRange(0, SpawnPoints.Num() - 1);

	if (SpawnPoints.IsValidIndex(SpawnPointIndex))
	{
		return SpawnPoints[SpawnPointIndex];
	}

	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "GetRandomSpawnPoint: SpanPointIndex invalid!");
		return nullptr;
	}
}

void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int ASpawnManager::GetCurrentRound() const
{
	return CurrentRound;
}


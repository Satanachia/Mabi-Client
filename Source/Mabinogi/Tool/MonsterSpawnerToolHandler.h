// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawnerToolHandler.generated.h"

class UMonsterSpawnerToolComponent;

UCLASS()
class MABINOGI_API AMonsterSpawnerToolHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	AMonsterSpawnerToolHandler();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void Load();
	void Save();
	UMonsterSpawnerToolComponent* CreatePivot(int32 _Index);

	void GetSpawnerComponents(OUT TArray<UMonsterSpawnerToolComponent*>& _Spanwers);

public:
	UPROPERTY(EditAnywhere)
	bool SaveBtn = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABattleLevel> Level;

private:
	float Timer = 0.f;
	const float Duration = 3.f;
	FString FolderPath;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MonsterSpawnerToolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MABINOGI_API UMonsterSpawnerToolComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UMonsterSpawnerToolComponent();
	void Load(FArchive* _FileReaderAr);
	void Save(FArchive* _FileWriterAr);

	void DebugRender();

private:
	FVector LoadLocation(FArchive* _FileReaderAr);
	void SaveLocation(const FVector& _Location, FArchive* _FileWriterAr);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnmovableZone.generated.h"

UCLASS()
class MABINOGI_API AUnmovableZone : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnmovableZone();

public:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* Zone = nullptr;
};

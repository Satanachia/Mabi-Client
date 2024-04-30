// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WalkableArea.generated.h"

UCLASS()
class MABINOGI_API AWalkableArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AWalkableArea();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComponent = nullptr;
};

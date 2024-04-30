// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleWidget.generated.h"

/**
 * 
 */
UCLASS()
class MABINOGI_API UBattleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void DrawLine(const FVector2D& _Start, const FVector2D& _End);
	void DrawClear() { LineColor = FColor::Transparent; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D LineStartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D LineEndPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FColor LineColor = FColor::Transparent;
};

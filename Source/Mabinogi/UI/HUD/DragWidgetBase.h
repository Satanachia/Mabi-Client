// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragWidgetBase.generated.h"

class UUserWidget;


/**
 * 
 */
UCLASS()
class MABINOGI_API UDragWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* AfterImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D DragOffset;
};

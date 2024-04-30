// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/DragWidgetBase.h"
#include "Protocol.pb.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */

class UInvenItemWidget;
class UTexture2D;
class USlotActionBase;

UCLASS()
class MABINOGI_API UInventoryWidget : public UDragWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	
private:

};

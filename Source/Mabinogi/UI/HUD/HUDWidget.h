// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/DragAndDrop.h"
#include "HUDWidget.generated.h"

class UActionSlotWidget;
class USkillInfoWidget;
class UInventoryWidget;
class USlotWidget;

/**
 * 
 */
UCLASS()
class MABINOGI_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ExcuteActionSlot(int32 _Index);
	void OnOffSkillWidget();
	void OnOffInventory();

	USkillInfoWidget* GetSkillUI() { return SkillUI; }
	UInventoryWidget* GetInvenUI() { return InvenUI; }

	UActionSlotWidget* FindActionSlot(USlotWidget* _Target, UActionSlotWidget* _Ignore = nullptr);
	
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	

private:
	enum { SLOT_MAX_COUNT = 5 };
	UActionSlotWidget* ActionSlots[SLOT_MAX_COUNT];
	USkillInfoWidget* SkillUI = nullptr;
	UInventoryWidget* InvenUI = nullptr;
};

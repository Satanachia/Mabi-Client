// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/HUDWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "UI/HUD/ActionSlotWidget.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "UI/MouseCursorWidget.h"
#include "UI/HUD/SkillInfoWidget.h"
#include "UI/HUD/InventoryWidget.h"



void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	const FString ActionSlotName = TEXT("WBP_ActionSlot");
	for (int32 i = 0; i < SLOT_MAX_COUNT; ++i)
	{
		const FName SlotName = *(ActionSlotName + FString::FromInt(i));
		ActionSlots[i] = Cast<UActionSlotWidget>(GetWidgetFromName(SlotName));
		check(ActionSlots[i]);
		ActionSlots[i]->InitSlotNumber(i);
	}
	
	SkillUI = Cast<USkillInfoWidget>(GetWidgetFromName(TEXT("WBP_SkillInfo")));
	check(SkillUI);
	InvenUI = Cast<UInventoryWidget>(GetWidgetFromName(TEXT("WBP_Inventory")));
	check(InvenUI);
}

FReply UHUDWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Result = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	ANetworkPlayerController* PlayerCtrl = Cast<ANetworkPlayerController>(GetWorld()->GetFirstPlayerController());
	check(PlayerCtrl);
	UMouseCursorWidget* MouseCursorWidget = PlayerCtrl->GetCursorWidget();

	USlotWidget* MovingSlot = MouseCursorWidget->GetMovingSlot();
	if (nullptr != MovingSlot)
	{
		//마우스커서가 지금 뭔가를 붙잡고 있는 경우
		MovingSlot->CancelMove();
	}
	MouseCursorWidget->ChangeDefaultCursor();
	return Result;
}


void UHUDWidget::ExcuteActionSlot(int32 _Index)
{
	check(0 <= _Index && _Index < SLOT_MAX_COUNT);
	
	ActionSlots[_Index]->ExcuteAction();
}

void UHUDWidget::OnOffSkillWidget()
{
	if (true == SkillUI->GetIsEnabled())
	{
		SkillUI->SetVisibility(ESlateVisibility::Hidden);
		SkillUI->SetIsEnabled(false);
	}
	else
	{
		SkillUI->SetVisibility(ESlateVisibility::Visible);
		SkillUI->SetIsEnabled(true);
	}
}

void UHUDWidget::OnOffInventory()
{
	if (true == InvenUI->GetIsEnabled())
	{
		InvenUI->SetVisibility(ESlateVisibility::Hidden);
		InvenUI->SetIsEnabled(false);
	}
	else
	{
		InvenUI->SetVisibility(ESlateVisibility::Visible);
		InvenUI->SetIsEnabled(true);
	}
}


UActionSlotWidget* UHUDWidget::FindActionSlot(USlotWidget* _Target, UActionSlotWidget* _Ignore /*= nullptr*/)
{
	UActionSlotWidget* FindSlot = nullptr;

	for (int32 i = 0; i < SLOT_MAX_COUNT; ++i)
	{
		UActionSlotWidget* ActionSlot = ActionSlots[i];
		if (ActionSlot == _Ignore)
			continue;

		if (ActionSlot->GetTarget() != _Target)
			continue;

		check(nullptr == FindSlot);	//2개 이상 중복인지 확인
		FindSlot = ActionSlot;
	}

	return FindSlot;
}
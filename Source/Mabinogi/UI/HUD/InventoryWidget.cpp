// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/InventoryWidget.h"
#include "Components/GridPanel.h"
#include "UI/HUD/InvenItemWidget.h"
#include "MabiGameInstance.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UGridPanel* Grid = Cast<UGridPanel>(GetWidgetFromName(TEXT("ItemGrid")));
	
	TArray<UWidget*> GridChildren = Grid->GetAllChildren();

	TArray<UInvenItemWidget*> ItemSlots;
	ItemSlots.Reserve(GridChildren.Num());
	for (int32 i = 0; i < GridChildren.Num(); ++i)
	{
		UInvenItemWidget* ItemSlot = Cast<UInvenItemWidget>(GridChildren[i]);
		check(ItemSlot);
		ItemSlots.Add(ItemSlot);
		ItemSlot->SetInventoryIndex(i);
	}

	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GetGameInstance());
	check(GameInst);
	GameInst->InventoryMgr.Init(std::move(ItemSlots));
	
	SetVisibility(ESlateVisibility::Hidden);
	SetIsEnabled(false);
}

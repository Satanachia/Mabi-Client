// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/InvenItemWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/MouseCursorWidget.h"
#include "UI/HUD/HUDWidget.h"
#include "UI/HUD/InventoryWidget.h"
#include "UI/HUD/ActionSlotWidget.h"
#include "Managers/ResourceManager.h"
#include "Mabinogi.h"

void UInvenItemWidget::LeaveSlot(USlotWidget* _MovingSlot)
{
	Super::LeaveSlot(_MovingSlot);
	GetSlotImg()->SetOpacity(0.5f);
}

void UInvenItemWidget::EnterSlot(USlotWidget* _MovingSlot)
{
	Super::EnterSlot(_MovingSlot);
	
	UInvenItemWidget* MovingItemSlot = Cast<UInvenItemWidget>(_MovingSlot);
	if (nullptr == MovingItemSlot)
		return;

	InventoryManager& InvenMgr = GetInventoryMgr();
	if (true == IsEmpty())
	{
		InvenMgr.MoveItem(MovingItemSlot, this);
		return;
	}
	
	if (this == MovingItemSlot)
		return;
	
	InvenMgr.SwapItem(MovingItemSlot, this);
}

bool UInvenItemWidget::IsEmpty()
{
	bool Result = Super::IsEmpty();
	Result &= GetInventoryMgr().IsEmpty(Index);
	return Result;
}

void UInvenItemWidget::CancelMove()
{
	Super::CancelMove();
	GetInventoryMgr().DropItem(this);
}

void UInvenItemWidget::ExcuteAction()
{
	Super::ExcuteAction();

	InventoryManager& InvenMgr = GetInventoryMgr();
	InvenMgr.UseItem(this);
}

void UInvenItemWidget::SetInventoryIndex(int32 _Index)
{
	Index = _Index;
	check(0 <= Index);
}

int32 UInvenItemWidget::GetInventoryIndex()
{
	check(0 <= Index);
	return Index;
}

void UInvenItemWidget::Alter(const Protocol::ItemInfo& _Data)
{
	Protocol::ItemType ItemType = _Data.type();
	int32 Count = _Data.count();
	
	float ImgAlpha = 0.f;
	UTexture2D* Texture = nullptr;
	if (0 < Count && Protocol::ITEM_TYPE_NONE != ItemType)
	{
		Texture = ResourceManager::GetTexture(ItemType);
		ImgAlpha = 1.f;
	}
	
	SetTexture(Texture, ImgAlpha);
	SetCountValue(Count);
}

UActionSlotWidget* UInvenItemWidget::FindBindingSlot()
{
	return GetHud()->FindActionSlot(this);
}

InventoryManager& UInvenItemWidget::GetInventoryMgr()
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GetGameInstance());
	check(GameInst);
	return GameInst->InventoryMgr;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/SlotWidget.h"
#include "InvenItemWidget.generated.h"

class InventoryManager;
class UActionSlotWidget;

/**
 * 
 */
UCLASS()
class MABINOGI_API UInvenItemWidget : public USlotWidget
{
	GENERATED_BODY()

protected:
	virtual void LeaveSlot(USlotWidget* _MovingSlot) override;
	virtual void EnterSlot(USlotWidget* _MovingSlot) override;

	virtual bool IsEmpty() override;
	virtual void CancelMove() override;
	virtual void ExcuteAction() override;

public:
	void SetInventoryIndex(int32 _Index);
	int32 GetInventoryIndex();
	void Alter(const Protocol::ItemInfo& _Data);
	
	UActionSlotWidget* FindBindingSlot();

private:
	class InventoryManager& GetInventoryMgr();

private:
	int32 Index = -1;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/SlotWidget.h"
#include "ActionSlotWidget.generated.h"

class UTexture2D;

/**
 * 
 */
UCLASS()
class MABINOGI_API UActionSlotWidget : public USlotWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void InitSlotNumber(int32 _Num);
	virtual void ExcuteAction() override;

	USlotWidget* GetTarget() { return Target; }
	void Copy(USlotWidget* _Target);

protected:
	virtual void LeaveSlot(USlotWidget* _MovingSlot) override;
	virtual void EnterSlot(USlotWidget* _MovingSlot) override;
	virtual bool IsEmpty() override;
	virtual void CancelMove() override;

private:
	
private:
	class UTextBlock* NameText;
	USlotWidget* Target = nullptr;
};

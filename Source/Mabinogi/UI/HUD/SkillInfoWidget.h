// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/DragWidgetBase.h"
#include "Protocol.pb.h"
#include "SkillInfoWidget.generated.h"

class USkillSlotWidget;

/**
 * 
 */
UCLASS()
class MABINOGI_API USkillInfoWidget : public UDragWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

public:
	void Init(Protocol::CreatureType _PlayerType);
	
private:
	bool BindSlot(const FName& _Name, Protocol::AttackType _SkillType);

private:
	TArray<USkillSlotWidget*> SkillSlots;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/SlotWidget.h"
#include "Protocol.pb.h"
#include "SkillSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class MABINOGI_API USkillSlotWidget : public USlotWidget
{
	GENERATED_BODY()
	
public:
	void Init(Protocol::AttackType _Type);
	void OffSlot();
	Protocol::CreatureType GetOwnerType();

protected:
	virtual void ExcuteAction() override;

private:
	UTexture2D* CreateTexture(Protocol::AttackType _Type);

private:
	Protocol::AttackType AttackType = Protocol::ATTACK_TYPE_NONE;
};

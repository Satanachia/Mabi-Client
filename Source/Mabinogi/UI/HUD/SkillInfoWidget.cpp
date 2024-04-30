// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SkillInfoWidget.h"
#include "UI/HUD/SkillSlotWidget.h"

void USkillInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(BindSlot(TEXT("Skill_Metro"), Protocol::ATTACK_TYPE_SKILL_MAGE));
	check(BindSlot(TEXT("Skill_Rush"), Protocol::ATTACK_TYPE_SKILL_WARRIOR));
	check(BindSlot(TEXT("Skill_Blink"), Protocol::ATTACK_TYPE_SKILL_ROG));
}

bool USkillInfoWidget::BindSlot(const FName& _Name, Protocol::AttackType _SkillType)
{
	USkillSlotWidget* SkillSlot = Cast<USkillSlotWidget>(GetWidgetFromName(_Name));
	check(SkillSlot);

	if (true == SkillSlots.Contains(SkillSlot))
		return false;
	
	SkillSlots.Add(SkillSlot);
	SkillSlot->Init(_SkillType);
	return true;
}

void USkillInfoWidget::Init(Protocol::CreatureType _PlayerType)
{
	for (USkillSlotWidget* SkillSlot : SkillSlots)
	{
		bool IsActive = (SkillSlot->GetOwnerType() == _PlayerType);
		if (true == IsActive)
			continue;
		
		SkillSlot->OffSlot();
	}
}

void USkillInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Hidden);
	SetIsEnabled(false);
}





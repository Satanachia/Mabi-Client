// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SkillSlotWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Mabinogi.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "Actors/Network/NetworkPlayer.h"
#include "UI/ObjectInfoWidget.h"
#include "Managers/ResourceManager.h"


void USkillSlotWidget::Init(Protocol::AttackType _Type)
{
	AttackType = _Type;
	UTexture2D* Texture = CreateTexture(AttackType);
	SetTexture(Texture);
}


UTexture2D* USkillSlotWidget::CreateTexture(Protocol::AttackType _Type)
{
	UTexture2D* Texture = ResourceManager::GetTexture(_Type);
	check(Texture);
	return Texture;
}



void USkillSlotWidget::OffSlot()
{
	GetSlotImg()->SetOpacity(0.5f);
	GetSlotBtn()->SetIsEnabled(false);
}

Protocol::CreatureType USkillSlotWidget::GetOwnerType()
{
	Protocol::CreatureType Result = Protocol::CREATURE_TYPE_NONE;

	switch (AttackType)
	{
	case Protocol::ATTACK_TYPE_SKILL_MAGE:
		Result = Protocol::CREATURE_TYPE_PLAYER_MAGE;
		break;
	case Protocol::ATTACK_TYPE_SKILL_WARRIOR:
		Result = Protocol::CREATURE_TYPE_PLAYER_WARRIOR;
		break;
	case Protocol::ATTACK_TYPE_SKILL_ROG:
		Result = Protocol::CREATURE_TYPE_PLAYER_ROG;
		break;
	}
	
	check(Protocol::CREATURE_TYPE_NONE != Result);
	return Result;
}

void USkillSlotWidget::ExcuteAction()
{
	Super::ExcuteAction();

	check(Protocol::ATTACK_TYPE_NONE != AttackType);
	check(Protocol::ATTACK_TYPE_COUNT != AttackType);
	
	ANetworkPlayerController* PlayerCtrl = Cast<ANetworkPlayerController>(GetWorld()->GetFirstPlayerController());
	check(PlayerCtrl);
	ANetworkPlayer* Player = PlayerCtrl->GetPlayer();
	UObjectInfoWidget* InfoUI = Player->GetInfoUI();
	if (false == InfoUI->IsNoThink())
		return;
	
	Protocol::C_THINK reqPkt;
	reqPkt.set_attack_type(AttackType);
	SEND_PACKET(reqPkt);
	
	UTexture2D* SlotTexture = GetTexture();
	InfoUI->StartThink(AttackType, SlotTexture);

	ResourceManager::PlaySound(SoundType::ThinkReady, GetWorld());
}



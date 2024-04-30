// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInst/PlayerAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Mabinogi.h"
#include "Actors/Network/NetworkCreature.h"
#include "UI/ObjectInfoWidget.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "Managers/ResourceManager.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OnMontageEnded.AddDynamic(this, &UPlayerAnimInstance::OnMontageEnd);
}

void UPlayerAnimInstance::PlayAniPacketSound(Protocol::AnimationType _Type)
{
	UWorld* World = GetWorld();
	switch (_Type)
	{
	case Protocol::ANI_TYPE_NORMAL_ATTACK:
		ResourceManager::PlaySound(SoundType::AttackSwing, World);
		break;
	case Protocol::ANI_TYPE_SKILL:
		break;
	case Protocol::ANI_TYPE_DAMAGED:
		ResourceManager::PlaySound(SoundType::Hit, World);
		break;
	case Protocol::ANI_TYPE_DEATH:
		ResourceManager::PlaySound(SoundType::Hit, World);
		ResourceManager::PlaySound(SoundType::Coin, World);
		break;
	}
}

void UPlayerAnimInstance::HandleAnimationPacket(Protocol::AnimationType _Type)
{
	Super::HandleAnimationPacket(_Type);

	if (Protocol::ANI_TYPE_DAMAGED != _Type && Protocol::ANI_TYPE_DEATH != _Type)
		return;

	ANetworkCreature* NetCreature = Cast<ANetworkCreature>(TryGetPawnOwner());
	if (nullptr == NetCreature)
		return;

	if (false == NetCreature->IsMyPlayer())
		return;

	ANetworkPlayerController* PlayerCtrl = Cast<ANetworkPlayerController>(GetWorld()->GetFirstPlayerController());
	if (nullptr == PlayerCtrl)
		return;
	
	PlayerCtrl->ShakeCamera();
}

void UPlayerAnimInstance::BindAttackNotify(AniFunctor&& _Callback)
{
	AttackNotifies.Add(_Callback);
}

void UPlayerAnimInstance::BindMontageFinish(AniFunctor&& _Callback)
{
	FinishNotifies.Add(_Callback);
}

void UPlayerAnimInstance::AnimNotify_Attack()
{
	for (AniFunctor& Func : AttackNotifies)
	{
		Func();
	}
}

void UPlayerAnimInstance::Play_AttackMontage(UAnimMontage* _AttackMontage)
{
	const float ComboWaitTime = 1.f;
	float NowAttackTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	if (ComboWaitTime < (NowAttackTime - PrevAttackTime))
	{
		AttackCombo = 0;
	}
	else
	{
		AttackCombo = (AttackCombo + 1) % MAX_COMBO;
	}
	PrevAttackTime = NowAttackTime;

	static const FString SectionStr = TEXT("NormalAttack");
	const FName SectionName = FName(*(SectionStr + FString::FromInt(AttackCombo)));
	PlayMontage(_AttackMontage);
	Montage_JumpToSection(SectionName, _AttackMontage);
}

void UPlayerAnimInstance::OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	for (AniFunctor& Func : FinishNotifies)
	{
		Func();
	}

	if (SkillMontage != Montage)
		return;

	ANetworkCreature* NetCreature = Cast<ANetworkCreature>(TryGetPawnOwner());
	check(NetCreature);
	NetCreature->GetInfoUI()->FadeOut();
}

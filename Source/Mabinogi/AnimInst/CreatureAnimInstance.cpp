// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInst/CreatureAnimInstance.h"
#include "Actors/Network/NetworkOthers.h"
#include "Mabinogi.h"
#include "UI/ObjectInfoWidget.h"
#include "Actors/Network/NetworkCreature.h"
#include "Define.h"

void UCreatureAnimInstance::NativeUpdateAnimation(float _DeltaTime)
{
	Super::NativeUpdateAnimation(_DeltaTime);

	APawn* Pawn = TryGetPawnOwner();
	if (nullptr == Pawn)
		return;

	FVector NowPos = Pawn->GetActorLocation();
	float Length = (NowPos - PrevPos).Length();
	IsMoving = (KINDA_SMALL_NUMBER < Length);
	PrevPos = NowPos;
}

void UCreatureAnimInstance::PlayMontage(UAnimMontage* _Montage, bool _IsReplay /*= false*/)
{
	check(_Montage);
	
	if (true != Montage_IsPlaying(_Montage))
	{
		Montage_Play(_Montage);
		return;
	}

	if (false == _IsReplay)
		return;
	
	Montage_Stop(0.0f, _Montage);
	Montage_Play(_Montage);
}


void UCreatureAnimInstance::HandleAnimationPacket(Protocol::AnimationType _Type)
{
	switch (_Type)
	{
	case Protocol::ANI_TYPE_NORMAL_ATTACK:
	{
		check(AttackMontage);
		Play_AttackMontage(AttackMontage);
		break;
	}
	case Protocol::ANI_TYPE_SKILL:
		PlayMontage(SkillMontage);
		break;
	case Protocol::ANI_TYPE_DAMAGED:
		PlayMontage(DamagedMontage, true);
		break;
	case Protocol::ANI_TYPE_DEATH:
		IsDeath = true;
		break;
	}

	LastMontage = _Type;

	APawn* OwnerActor = TryGetPawnOwner();
	check(OwnerActor);
	PlayAniPacketSound(_Type);
}

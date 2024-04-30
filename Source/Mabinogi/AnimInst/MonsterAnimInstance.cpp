// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInst/MonsterAnimInstance.h"
#include "Mabinogi.h"
#include "Managers/ResourceManager.h"

void UMonsterAnimInstance::NativeUpdateAnimation(float _DeltaTime)
{
	Super::NativeUpdateAnimation(_DeltaTime);

	if (true == IsMoving && false == PrevMoving)
	{
		ResourceManager::PlaySound(SoundType::ChickIdle, GetWorld());
	}
	
	PrevMoving = IsMoving;
}

void UMonsterAnimInstance::PlayAniPacketSound(Protocol::AnimationType _Type)
{
	UWorld* World = GetWorld();
	switch (_Type)
	{
	case Protocol::ANI_TYPE_NORMAL_ATTACK:
		ResourceManager::PlaySound(SoundType::ChickAttack, World);
		break;
	case Protocol::ANI_TYPE_SKILL:
		break;
	case Protocol::ANI_TYPE_DAMAGED:
		ResourceManager::PlaySound(SoundType::ChickDamaged, World);
		ResourceManager::PlaySound(SoundType::Hit, World);
		break;
	case Protocol::ANI_TYPE_DEATH:
		ResourceManager::PlaySound(SoundType::Hit, World);
		ResourceManager::PlaySound(SoundType::ChickDeath, World);
		ResourceManager::PlaySound(SoundType::Coin, World);
		break;
	}
}

void UMonsterAnimInstance::Play_AttackMontage(UAnimMontage* _AttackMontage)
{
	PlayMontage(_AttackMontage);
}

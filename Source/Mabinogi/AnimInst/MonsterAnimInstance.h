// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInst/CreatureAnimInstance.h"
#include "MonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MABINOGI_API UMonsterAnimInstance : public UCreatureAnimInstance
{
	GENERATED_BODY()
	
public:

protected:
	virtual void NativeUpdateAnimation(float _DeltaTime) override;
	virtual void PlayAniPacketSound(Protocol::AnimationType _Type) override;
	virtual void Play_AttackMontage(UAnimMontage* _AttackMontage) override;

protected:
	bool PrevMoving = false;
};

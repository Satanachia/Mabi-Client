// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInst/CreatureAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

//DECLARE_DELEGATE(FAniDelegate)
using AniFunctor = TFunction<void()>;
using FAniDelegate = TArray<AniFunctor>;


/**
 * 
 */
UCLASS()
class MABINOGI_API UPlayerAnimInstance : public UCreatureAnimInstance
{
	GENERATED_BODY()

public:
	virtual void HandleAnimationPacket(Protocol::AnimationType _Type) override;

	void BindAttackNotify(AniFunctor&& _Callback);
	void BindMontageFinish(AniFunctor&& _CallBack);

	int32 GetCurrentAttackCombo() { return AttackCombo; }
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void PlayAniPacketSound(Protocol::AnimationType _Type) override;
	virtual void Play_AttackMontage(UAnimMontage* _AttackMontage) override;

private:
	UFUNCTION()
	void AnimNotify_Attack();

	UFUNCTION()
	void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);

private:
	enum { MIN_COMBO = 0, MAX_COMBO = 3 };

	FAniDelegate AttackNotifies;
	FAniDelegate FinishNotifies;

	int32 AttackCombo = 0;
	float PrevAttackTime = 0.f;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Protocol.pb.h"
#include "CreatureAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MABINOGI_API UCreatureAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void HandleAnimationPacket(Protocol::AnimationType _Type);

protected:
	virtual void NativeUpdateAnimation(float _DeltaTime) override;

	virtual void PlayAniPacketSound(Protocol::AnimationType _Type) PURE_VIRTUAL(UCreatureAnimInstance::PlayAniPacketSound, );
	virtual void Play_AttackMontage(UAnimMontage* _AttackMontage) PURE_VIRTUAL(UCreatureAnimInstance::Play_AttackMontage, );
	void PlayMontage(UAnimMontage* _Montage, bool _IsReplay = false);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsMoving = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* SkillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DamagedMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsDeath = false;

private:
	
private:
	FVector PrevPos;
	Protocol::AnimationType LastMontage = Protocol::ANI_TYPE_NONE;
};

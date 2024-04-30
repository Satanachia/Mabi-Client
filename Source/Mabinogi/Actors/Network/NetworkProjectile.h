// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Network/NetworkObject.h"
#include "NetworkProjectile.generated.h"

/**
 * 
 */
UCLASS()
class MABINOGI_API ANetworkProjectile : public ANetworkObject
{
	GENERATED_BODY()
	
public:
	ANetworkProjectile();

protected:
	virtual void InitFromType(uint32 _NetworkSecondType) override;
	virtual void Tick(float _DeltaTime) override;
	virtual void SetMovePkt(Protocol::S_MOVE& pkt) override;
	
private:
	static TMap<Protocol::AttackType, class UParticleSystem*> ParticleAssets;

	Protocol::AttackType AttackType = Protocol::ATTACK_TYPE_NONE;
	UParticleSystemComponent* Particle = nullptr;
	
	const float Duration = 0.1f;
	float Timer = -1.f;
	FVector MoveStartLocation = FVector::ZeroVector;
	FVector MoveDestLocation = FVector::ZeroVector;
};

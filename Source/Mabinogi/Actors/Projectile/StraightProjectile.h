// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StraightProjectile.generated.h"

UCLASS()
class MABINOGI_API AStraightProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AStraightProjectile();

	void Init(AActor* _Target, const FVector& _LaunchLocation);
	void SetTarget(AActor* _Target) { TargetPtr = _Target;}

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	class UParticleSystem* ProjectileTemplate;
	class UParticleSystemComponent* ProjectileParticle;
	class UParticleSystem* ExplosionTemplate;
	class UParticleSystemComponent* ExplosionParticle;

	TWeakObjectPtr<AActor> TargetPtr;
	FVector LaunchLocation;

	float Timer = 0.f;
	const float Duration = 0.5f;
	bool IsAttached = false;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Network/NetworkObject.h"
#include "NetworkCreature.generated.h"

class UCreatureAnimInstance;
class UParticleSystemComponent;

enum class CreatureParticleType
{
	HitReact,
	Death,

	COUNT
};

/**
 * 
 */
UCLASS()
class MABINOGI_API ANetworkCreature : public ANetworkObject
{
	GENERATED_BODY()

public:
	ANetworkCreature();

protected:
	virtual void InitFromType(uint32 _NetworkSecondType) override;

public:
	void SetHp(int32 _Value);
	class UObjectInfoWidget* GetInfoUI();

	void PlayParticle(CreatureParticleType _Type);
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float _DeltaTime) override;

private:
	void CreateParticleComponent(CreatureParticleType _Type, const FName& _Name, class UParticleSystem* _Helper, const FVector& _RelativeLocation = FVector::ZeroVector);
	
public:
	static const FName AttackSocketName;
	const float MoveSpeed = 500.f;

private:
	static TArray<TObjectPtr<USkeletalMesh>> AllMeshRes;
	static TArray<TSubclassOf<UCreatureAnimInstance>>AllAnimInstClass;
	static double InfoVisibleLength;

	class UWidgetComponent* InfoUI = nullptr;
	class USceneComponent* AttackSocket = nullptr;
	
	UParticleSystemComponent* Particles[static_cast<int32>(CreatureParticleType::COUNT)];
};

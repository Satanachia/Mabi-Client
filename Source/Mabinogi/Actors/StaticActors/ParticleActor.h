// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Protocol.pb.h"
#include "ParticleActor.generated.h"

UCLASS()
class MABINOGI_API AParticleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AParticleActor();

	void Init(const Protocol::S_SPAWN_PARTICLE& _Pkt);

protected:
	virtual void Tick(float DeltaTime) override;

	
private:
	static class UParticleSystem* ParticleAssets[Protocol::PARTICLE_TYPE_COUNT];

	class UParticleSystemComponent* Particle = nullptr;
	bool IsRun = false;
};

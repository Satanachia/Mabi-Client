// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Network/NetworkCreature.h"
#include "NetworkOthers.generated.h"

/**
 * 
 */
UCLASS()
class MABINOGI_API ANetworkOthers : public ANetworkCreature
{
	GENERATED_BODY()
	
public:
	ANetworkOthers();

	virtual void SetMovePkt(Protocol::S_MOVE& pkt) override;
	
protected:
	virtual void LookAt(const FVector& _Pos) override;
	virtual void Tick(float _DeltaTime) override;
	
private:
	void SetDestination(const FVector& _Dest);

private:
	float MovingTimer = -1.f;
	FVector StartLocation;
	FVector Destination;
	FRotator StartRot;
	FRotator DestRot;
};

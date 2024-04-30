// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Network/NetworkOthers.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "Actors/Network/NetworkPlayer.h"
#include "AnimInst/CreatureAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Define.h"

ANetworkOthers::ANetworkOthers()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANetworkOthers::SetMovePkt(Protocol::S_MOVE& pkt)
{
	Super::SetMovePkt(pkt);

	const Protocol::PosInfo& DestPos = GetProtocolPos();
	SetDestination(FVector(DestPos.x(), DestPos.y(), DestPos.z()));
}

void ANetworkOthers::LookAt(const FVector& _Pos)
{
	//Super::LookAt(_Pos);

	FVector PlayerLocation = GetActorLocation();
	FVector DirVec = (_Pos - PlayerLocation);
	DirVec.Normalize();

	SetDestination(GetActorLocation() + DirVec);
}

void ANetworkOthers::SetDestination(const FVector& _Dest)
{
	MovingTimer = 0.f;
	StartLocation = GetActorLocation();
	Destination = _Dest;

	float Yaw = (Destination - StartLocation).Rotation().Yaw;
	StartRot = GetActorRotation();
	DestRot = FRotator(0.f, Yaw, 0.f);
}


void ANetworkOthers::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	
	if (MovingTimer < 0.f)
		return;

	MovingTimer += _DeltaTime;
	float Ratio = FMath::Min((MovingTimer / NET_CREATURE_MOVE_DURATION), 1.f);

	FVector NowLocation = FMath::Lerp(StartLocation, Destination, Ratio);
	SetActorLocation(NowLocation);

	FRotator NowRot = FMath::Lerp(StartRot, DestRot, Ratio);
	SetActorRotation(NowRot);
}


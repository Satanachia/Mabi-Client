// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LobbyActorBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

ALobbyActorBase::ALobbyActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = CapsuleComponent;

	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	check(Mesh);

	Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	Mesh->bCastDynamicShadow = true;
	Mesh->bAffectDynamicIndirectLighting = true;
	Mesh->SetupAttachment(CapsuleComponent);

	Mesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	Mesh->SetRelativeRotation(FRotator(0.f, -88.f, 0.f));

	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	SetAniState(LobbyActorState::Idle);
}

void ALobbyActorBase::SetAniState(LobbyActorState _State)
{
	if (_State == CurState)
		return;

	CurState = _State;
	switch (_State)
	{
	case LobbyActorState::Idle:
		Mesh->PlayAnimation(IdleAnimation, true);
		break;
	case LobbyActorState::Hover:
		Mesh->PlayAnimation(HoverAnimation, true);
		break;
	case LobbyActorState::Selected:
		Mesh->PlayAnimation(SelectedAnimation, true);
		break;
	default:
		break;
	}
}




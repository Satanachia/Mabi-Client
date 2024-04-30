// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LobbyPawn.h"
#include "Camera/CameraComponent.h"
#include "Actors/LobbyActorBase.h"
#include "AnimInst/LobbyAnimInstance.h"

ALobbyPawn::ALobbyPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Cam->SetupAttachment(RootComponent);
	Cam->bUsePawnControlRotation = false;
}

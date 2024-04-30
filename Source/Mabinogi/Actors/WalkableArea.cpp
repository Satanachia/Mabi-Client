// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/WalkableArea.h"
#include "Components/BoxComponent.h"
#include "Actors/Network/NetworkPlayer.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Protocol.pb.h"

AWalkableArea::AWalkableArea()
{
	PrimaryActorTick.bCanEverTick = false;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    check(BoxComponent);

    BoxComponent->SetupAttachment(RootComponent);
    BoxComponent->SetCollisionProfileName("BlockAll");
}


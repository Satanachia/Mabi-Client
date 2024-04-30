// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/StaticActors/UnmovableZone.h"
#include "Components/BoxComponent.h"

AUnmovableZone::AUnmovableZone()
{
	PrimaryActorTick.bCanEverTick = false;

	Zone = CreateDefaultSubobject<UBoxComponent>(TEXT("Zone"));
	Zone->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
	RootComponent = Zone;
}


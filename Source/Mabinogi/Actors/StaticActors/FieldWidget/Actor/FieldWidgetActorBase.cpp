// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/StaticActors/FieldWidget/Actor/FieldWidgetActorBase.h"
#include "Components/BoxComponent.h"
#include "Actors/Network/NetworkPlayer.h"
#include "Levels/BaseLevel.h"
#include "Components/WidgetComponent.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "Components/Button.h"

AFieldWidgetActorBase::AFieldWidgetActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot"));
	RootComponent = Pivot;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(RootComponent);
	BoxCollider->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
	
	FieldWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FieldWidget"));
	FieldWidget->SetupAttachment(RootComponent);
	FieldWidget->SetWidgetSpace(EWidgetSpace::Screen);
}



void AFieldWidgetActorBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	FieldWidget->InitWidget();
}

void AFieldWidgetActorBase::ExcuteLClick(ANetworkPlayerController* _PlayerCtrl)
{
	if (true == IsCollision(_PlayerCtrl->GetPlayer()))
	{
		Excute(_PlayerCtrl);
	}
	else
	{
		_PlayerCtrl->MoveToPoint(GetActorLocation());
	}
}



bool AFieldWidgetActorBase::IsCollision(AActor* _Target)
{
	FVector TargetLocation = _Target->GetActorLocation();
	FBox Box = BoxCollider->Bounds.GetBox();
	return Box.IsInsideOrOn(TargetLocation);
}

UUserWidget* AFieldWidgetActorBase::GetUserWidget()
{
	UUserWidget* Widget = FieldWidget->GetUserWidgetObject();
	check(Widget);
	return Widget;
}
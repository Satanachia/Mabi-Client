// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Network/NetworkPlayer.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "Mabinogi.h"
#include "Components/CapsuleComponent.h"
#include "Actors/WalkableArea.h"
#include "Levels/BaseLevel.h"
#include "Actors/Controllers/NetworkPlayerController.h"

ANetworkPlayer::ANetworkPlayer()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 800.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm);
	FollowCamera->bUsePawnControlRotation = false;

	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
}

void ANetworkPlayer::InitFromType(uint32 _NetworkSecondType)
{
	Super::InitFromType(_NetworkSecondType);

	Protocol::CreatureType CreatureType = static_cast<Protocol::CreatureType>(_NetworkSecondType);
	bool IsPlayerType = false;
	IsPlayerType |= (CreatureType == Protocol::CREATURE_TYPE_PLAYER_WARRIOR);
	IsPlayerType |= (CreatureType == Protocol::CREATURE_TYPE_PLAYER_MAGE);
	IsPlayerType |= (CreatureType == Protocol::CREATURE_TYPE_PLAYER_ROG);
	check(IsPlayerType);
	
	ANetworkPlayerController* Ctrl = Cast<ANetworkPlayerController>(GetController());
	check(Ctrl);
	Ctrl->Init(this, CreatureType);

	
	FString UnrealString = FString(UTF8_TO_TCHAR(GetProtocolName().c_str()));
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GetGameInstance());
	check(GameInst);
	GameInst->SetMyPlayer(this);
	GameInst->InventoryMgr.LoadInventory(this);
}

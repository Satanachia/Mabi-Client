// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LobbyWatiActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimationAsset.h"

ALobbyWatiActor::ALobbyWatiActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	check(Mesh);
	
	Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	Mesh->bCastDynamicShadow = true;
	Mesh->bAffectDynamicIndirectLighting = true;
	Mesh->SetupAttachment(RootComponent);

	Mesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	Mesh->SetRelativeRotation(FRotator(0.f, -88.f, 0.f));
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>  MageMeshHelper(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSerath/Characters/Heroes/Serath/Meshes/Serath.Serath'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>  WarriorMeshHelper(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>  RogMeshHelper(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonShinbi/Characters/Heroes/Shinbi/Meshes/Shinbi.Shinbi'"));
	check(MageMeshHelper.Succeeded());
	check(WarriorMeshHelper.Succeeded());
	check(RogMeshHelper.Succeeded());
	
	static ConstructorHelpers::FObjectFinder<UAnimationAsset>  MageAniHelper(TEXT("/Script/Engine.AnimSequence'/Game/ParagonSerath/Characters/Heroes/Serath/Animations/Idle.Idle'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset>  WarriorAniHelper(TEXT("/Script/Engine.AnimSequence'/Game/ParagonGreystone/Characters/Heroes/Greystone/Animations/Idle.Idle'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset>  RogAniHelper(TEXT("/Script/Engine.AnimSequence'/Game/ParagonShinbi/Characters/Heroes/Shinbi/Animations/Idle.Idle'"));
	check(MageAniHelper.Succeeded());
	check(WarriorAniHelper.Succeeded());
	check(RogAniHelper.Succeeded());

	MeshRes.Add(MakeTuple(Protocol::CREATURE_TYPE_PLAYER_MAGE, MageMeshHelper.Object));
	MeshRes.Add(MakeTuple(Protocol::CREATURE_TYPE_PLAYER_WARRIOR, WarriorMeshHelper.Object));
	MeshRes.Add(MakeTuple(Protocol::CREATURE_TYPE_PLAYER_ROG, RogMeshHelper.Object));
	AnimInstClass.Add(MakeTuple(Protocol::CREATURE_TYPE_PLAYER_MAGE, MageAniHelper.Object));
	AnimInstClass.Add(MakeTuple(Protocol::CREATURE_TYPE_PLAYER_WARRIOR, WarriorAniHelper.Object));
	AnimInstClass.Add(MakeTuple(Protocol::CREATURE_TYPE_PLAYER_ROG, RogAniHelper.Object));
}

void ALobbyWatiActor::ChangeMesh(Protocol::CreatureType _type)
{
	USkeletalMesh* DesiredMesh = MeshRes[_type];
	UAnimationAsset* DesiredAni = AnimInstClass[_type];
	check(DesiredMesh);
	check(DesiredAni);
	
	Mesh->SetSkeletalMesh(DesiredMesh);
	Mesh->PlayAnimation(DesiredAni, true);
}



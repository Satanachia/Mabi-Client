// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Network/NetworkItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "Actors/Network/NetworkPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Managers/ResourceManager.h"
#include "Mabinogi.h"

ANetworkItem::ANetworkItem()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->DestroyComponent();	//스켈레탈 메시는 사용안함

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshHelper(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	check(MeshHelper.Object);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetStaticMesh(MeshHelper.Object);
	Mesh->SetRelativeRotation(FRotator(0.f, -88.f, 88.f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
}


void ANetworkItem::InitFromType(uint32 _NetworkSecondType)
{
	Super::InitFromType(_NetworkSecondType);

	Protocol::PosInfo& Pos = GetProtocolPos();
	float Height = CheckTerrainHeight(Pos);
	Pos.set_z(Height);
	SetActorLocation(FVector{ Pos.x(), Pos.y(), Pos.z() });
	
	Protocol::ItemType ItemType = static_cast<Protocol::ItemType>(_NetworkSecondType);
	SetMaterial(ItemType);
}


void ANetworkItem::SetMaterial(Protocol::ItemType _Type)
{
	UMaterialInterface* LoadedMaterial = ResourceManager::GetMaterial(_Type);
	Mesh->SetMaterial(0, LoadedMaterial);
}


void ANetworkItem::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == IsUsed)
		return;

	Move(_DeltaTime);
	Look();
}


void ANetworkItem::Move(float _DeltaTime)
{
	Timer += _DeltaTime;
	float SinValue = FMath::Sin(PI * Timer);
	FVector CurRelativeLocation = (FVector::UpVector * (HalfHeight * SinValue));
	Mesh->SetRelativeLocation(CurRelativeLocation);

	FVector LineStart = GetActorLocation();
	FVector LineEnd = LineStart + CurRelativeLocation;
	DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Green, false, 0.f);
}

void ANetworkItem::Look()
{
	ANetworkPlayerController* PlayerCtrl = Cast<ANetworkPlayerController>(GetWorld()->GetFirstPlayerController());
	if (nullptr == PlayerCtrl)
		return;

	UCameraComponent* Cam = PlayerCtrl->GetPlayer()->GetFollowCamera();
	FVector LookDir = Cam->GetComponentLocation() - GetActorLocation();
	LookDir.Normalize();
	SetActorRotation(LookDir.Rotation());
}

void ANetworkItem::ExcuteLClick(ANetworkPlayerController* _PlayerCtrl)
{
	if (true == IsUsed)
		return;

	if (true == IsCollision(_PlayerCtrl->GetPlayer()))
	{
		Excute(_PlayerCtrl);
	}
	
	_PlayerCtrl->MoveToPoint(GetActorLocation());
}



bool ANetworkItem::IsCollision(AActor* _Target)
{
	FVector ThisLocation = GetActorLocation();
	float Distance = (_Target->GetActorLocation() - ThisLocation).Length();
	bool Result = (Distance < CollisionRadius);

	FColor Color = Result ? FColor::Red : FColor::Green;
	float Duration = Result ? 1.f : 0.f;
	DrawDebugSphere(GetWorld(), ThisLocation, CollisionRadius, 12, Color, false, Duration);
	
	return Result;
}

void ANetworkItem::Excute(ANetworkPlayerController* _PlayerCtrl)
{
	Protocol::C_ADD_ITEM AddPkt;
	AddPkt.set_item_id(GetProtocolInfo().object_id());
	SEND_PACKET(AddPkt);
	
	IsUsed = true;
	Mesh->SetVisibility(false);
}

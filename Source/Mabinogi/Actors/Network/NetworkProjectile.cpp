// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Network/NetworkProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/CapsuleComponent.h"
#include "Levels/BattleLevel.h"

TMap<Protocol::AttackType, UParticleSystem*> ANetworkProjectile::ParticleAssets;

ANetworkProjectile::ANetworkProjectile()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> RushParticleHelper(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Skill_TeleCharge/P_Skill_Telecharge_Shock_Proj_03.P_Skill_Telecharge_Shock_Proj_03'"));
	if (true == ParticleAssets.IsEmpty())
	{
		check(RushParticleHelper.Succeeded());
		ParticleAssets.Add(Protocol::ATTACK_TYPE_SKILL_WARRIOR, RushParticleHelper.Object);
	}
	
	PrimaryActorTick.bCanEverTick = true;
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Particle->SetupAttachment(RootComponent);
	
	//충돌은 다 서버에서 할 예정
	GetCapsuleComponent()->SetActive(false);
	GetCapsuleComponent()->SetRelativeScale3D(FVector::OneVector);
}

void ANetworkProjectile::InitFromType(uint32 _NetworkSecondType)
{
	Super::InitFromType(_NetworkSecondType);
	
	FVector NowLocation = GetActorLocation();
	Protocol::PosInfo& PosInfo = GetProtocolPos();
	PosInfo.set_x(NowLocation.X);
	PosInfo.set_y(NowLocation.Y);
	PosInfo.set_z(NowLocation.Z);
	MoveStartLocation = NowLocation;
	SetActorLocation(MoveStartLocation);

	AttackType = static_cast<Protocol::AttackType>(_NetworkSecondType);
	switch (AttackType)
	{
	case Protocol::ATTACK_TYPE_SKILL_WARRIOR:
		Timer = 0.f;
		break;
	default:
		break;
	}
	
	UParticleSystem** FindIter = ParticleAssets.Find(AttackType);
	check(FindIter);
	Particle->SetTemplate(*FindIter);
	Particle->Activate();
}

void ANetworkProjectile::SetMovePkt(Protocol::S_MOVE& pkt)
{
	Super::SetMovePkt(pkt);
	
	/*ABattleLevel* Level = Cast<ABattleLevel>(GetWorld()->GetLevelScriptActor());
	check(Level);

	if (PosInfo.z() < MinHeight)
	{
		PosInfo.set_z(GetActorLocation().Z);
	}*/

	Protocol::PosInfo& PosInfo = GetProtocolPos();

	Timer = 0.f;
	MoveStartLocation = GetActorLocation();
	MoveDestLocation = FVector(PosInfo.x(), PosInfo.y(), PosInfo.z());
	float Yaw = (MoveDestLocation - MoveStartLocation).Rotation().Yaw;
	Particle->SetWorldRotation(FRotator(0.f, Yaw, 0.f));
}

void ANetworkProjectile::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (Timer < 0.f)
		return;
	
	Timer += _DeltaTime;
	float Ratio = FMath::Clamp(Timer / Duration, 0.f, 1.f);
	FVector CurLocation = FMath::Lerp(MoveStartLocation, MoveDestLocation, Ratio);
	SetActorLocation(CurLocation);
}
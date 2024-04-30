// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/StaticActors/ParticleActor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Actors/Network/NetworkObject.h"
#include "Levels/BattleLevel.h"
#include "MabiGameInstance.h"

UParticleSystem* AParticleActor::ParticleAssets[Protocol::PARTICLE_TYPE_COUNT] = { nullptr, };



AParticleActor::AParticleActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MagicZoneHelper(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Cines/Ausar/P_Ausar_Resurrection.P_Ausar_Resurrection'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MetroHelper(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Skill_TeleCharge/P_Skill_Telecharge_Shock_Impact_03.P_Skill_Telecharge_Shock_Impact_03'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> BlinkParticleHelper(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Archive/P_Teleport_Monster.P_Teleport_Monster'"));
	if (nullptr == ParticleAssets[Protocol::PARTICLE_TYPE_NONE + 1])
	{
		check(MagicZoneHelper.Succeeded());
		check(MetroHelper.Succeeded());
		check(BlinkParticleHelper.Succeeded());
		
		ParticleAssets[Protocol::PARTICLE_TYPE_MAGIC_ZONE] = MagicZoneHelper.Object;
		ParticleAssets[Protocol::PARTICLE_TYPE_METEO] = MetroHelper.Object;
		ParticleAssets[Protocol::PARTICLE_TYPE_BLINK] = BlinkParticleHelper.Object;

		for (int32 i = Protocol::PARTICLE_TYPE_NONE + 1; i < Protocol::PARTICLE_TYPE_COUNT; ++i)
		{
			check(ParticleAssets[i]);
		}
	}
	
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	RootComponent = Particle;
}

void AParticleActor::Init(const Protocol::S_SPAWN_PARTICLE& _Pkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GetWorld()->GetGameInstance());
	ABattleLevel* Level = Cast<ABattleLevel>(GameInst->GetCurrentLevel());
	check(Level);
	
	const Protocol::PosInfo& SpawnPos = _Pkt.position();
	float Z = ANetworkObject::CheckTerrainHeight(SpawnPos, Level);
	FVector SpawnLocation(SpawnPos.x(), SpawnPos.y(), Z);
	SetActorLocation(SpawnLocation);
	SetActorRotation(FRotator(0.f, _Pkt.yaw(), 0.f));
	
	Protocol::ParticleType ParticleType = _Pkt.particle_type();
	check(Protocol::PARTICLE_TYPE_NONE < ParticleType && ParticleType < Protocol::PARTICLE_TYPE_COUNT);
	check(ParticleAssets[ParticleType]);

	UParticleSystem* Asset = ParticleAssets[ParticleType];
	Particle->SetTemplate(Asset);
	Particle->Activate();
	IsRun = true;
}


void AParticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (false == IsRun)
		return;

	if (true == Particle->IsActive())
		return;

	Destroy();
}


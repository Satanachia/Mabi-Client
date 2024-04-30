// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile/StraightProjectile.h"
#include "Particles/ParticleSystemComponent.h"

AStraightProjectile::AStraightProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ProjectileHelper(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Gruntling/Master/P_ProjectileLob_Bomblet_01.P_ProjectileLob_Bomblet_01'"));
	check(ProjectileHelper.Succeeded());
	ProjectileTemplate = ProjectileHelper.Object;
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionHelper(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Gruntling/Master/P_ProjectileLob_BreakUp_01.P_ProjectileLob_BreakUp_01'"));
	check(ExplosionHelper.Succeeded());
	ExplosionTemplate = ExplosionHelper.Object;

	ProjectileParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileParticle"));
	RootComponent = ProjectileParticle;
	
	ExplosionParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionParticle"));
	ExplosionParticle->SetupAttachment(RootComponent);
}

void AStraightProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	check(ProjectileTemplate);
	ProjectileParticle->SetTemplate(ProjectileTemplate);
	ProjectileParticle->Activate(true);

	check(ExplosionTemplate);
	ExplosionParticle->SetTemplate(ExplosionTemplate);
	ExplosionParticle->Activate(false);
}

void AStraightProjectile::Init(AActor* _Target, const FVector& _LaunchLocation)
{
	TargetPtr = _Target;
	LaunchLocation = _LaunchLocation;
	SetActorLocation(LaunchLocation);
}

void AStraightProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (false == TargetPtr.IsValid())
	{
		Destroy();
		return;
	}

	AActor* Target = TargetPtr.Get();
	FVector TargetLocation = Target->GetActorLocation();
	
	Timer += DeltaTime;
	float Ratio = FMath::Clamp(Timer / Duration, 0.f, 1.f);
	FVector CurLocation = FMath::Lerp(LaunchLocation, TargetLocation, Ratio);
	SetActorLocation(CurLocation);
	
	if (Timer < Duration)
		return;
	
	if (false == IsAttached)
	{
		IsAttached = true;
		ProjectileParticle->Activate(false);
		ExplosionParticle->Activate(true);
	}
	
	if (true == ExplosionParticle->IsActive())
		return;
	
	Destroy();
}


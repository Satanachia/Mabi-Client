// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Network/NetworkCreature.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "AnimInst/CreatureAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "UI/ObjectInfoWidget.h"
#include "Particles/ParticleSystemComponent.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "Actors/Network/NetworkPlayer.h"
#include "Camera/CameraComponent.h"

//#include "Actors/Projectile/StraightProjectile.h"

TArray<TObjectPtr<USkeletalMesh>> ANetworkCreature::AllMeshRes;
TArray<TSubclassOf<UCreatureAnimInstance>> ANetworkCreature::AllAnimInstClass;
const FName ANetworkCreature::AttackSocketName = TEXT("socket_attack_pos");
double ANetworkCreature::InfoVisibleLength = 3000.0;

ANetworkCreature::ANetworkCreature()
{
	/*---------------------------------------------------
								�޽� �ҷ�����
	---------------------------------------------------*/
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshHelpers[Protocol::CREATURE_TYPE_COUNT] =
	{
		/*NONE*/		ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("/Script/Engine.SkeletalMesh'/Game/Market/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'")),
		/*MAGE*/		ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSerath/Characters/Heroes/Serath/Meshes/Serath.Serath'")),
		/*WARRIOR*/	ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'")),
		/*ROG*/			ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonShinbi/Characters/Heroes/Shinbi/Meshes/Shinbi.Shinbi'")),
		/*IGGY*/		ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonIggyScorch/Characters/Heroes/IggyScorch/Skins/Phoenix/Meshes/IggyScorch_Phoenix.IggyScorch_Phoenix'")),
		/*BOSS(������)*/			ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonIggyScorch/Characters/Heroes/IggyScorch/Skins/Phoenix/Meshes/IggyScorch_Phoenix.IggyScorch_Phoenix'"))
	};
	
	if (true == AllMeshRes.IsEmpty())
	{
		AllMeshRes.SetNum(Protocol::CREATURE_TYPE_COUNT);
		for (int32 i = 0; i < Protocol::CREATURE_TYPE_COUNT; ++i)
		{
			check(MeshHelpers[i].Succeeded());
			AllMeshRes[i] = MeshHelpers[i].Object;
		}
	}

	/*---------------------------------------------------
								�ִϸ��̼� �ν��Ͻ� Ŭ���� �ҷ�����
	---------------------------------------------------*/
	
	static ConstructorHelpers::FClassFinder<UCreatureAnimInstance> AnimHelpers[Protocol::CREATURE_TYPE_COUNT] =
	{
		/*NONE(������)*/		ConstructorHelpers::FClassFinder<UCreatureAnimInstance>(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/AnimInstance/ABP_Mage.ABP_Mage_C'")),
		/*MAGE*/		ConstructorHelpers::FClassFinder<UCreatureAnimInstance>(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/AnimInstance/ABP_Mage.ABP_Mage_C'")),
		/*WARRIOR*/	ConstructorHelpers::FClassFinder<UCreatureAnimInstance>(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/AnimInstance/ABP_Warrior.ABP_Warrior_C'")),
		/*ROG*/			ConstructorHelpers::FClassFinder<UCreatureAnimInstance>(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/AnimInstance/ABP_Rog.ABP_Rog_C'")),
		/*IGGY*/		ConstructorHelpers::FClassFinder<UCreatureAnimInstance>(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/AnimInstance/ABP_Iggy.ABP_Iggy_C'")),
		/*BOSS(������)*/			ConstructorHelpers::FClassFinder<UCreatureAnimInstance>(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/AnimInstance/ABP_Mage.ABP_Mage_C'"))
	};
	
	if (true == AllAnimInstClass.IsEmpty())
	{
		AllAnimInstClass.SetNum(Protocol::CREATURE_TYPE_COUNT);
		for (int32 i = 0; i < Protocol::CREATURE_TYPE_COUNT; ++i)
		{
			check(AnimHelpers[i].Succeeded());
			AllAnimInstClass[i] = AnimHelpers[i].Class;
		}
	}
	

	/*---------------------------------------------------
								������Ʈ �� ����
	---------------------------------------------------*/

	//��Ʈ�ѷ��� ĳ������ ���� ��ġ��Ű�� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -88.f, 0.f));

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	
	//�̵��� �� �ִ� �ӷ� ����
	MoveComp->MaxWalkSpeed = MoveSpeed;
	MoveComp->bOrientRotationToMovement = true;
	MoveComp->bUseControllerDesiredRotation = false;
	MoveComp->RotationRate = FRotator(0.f, 360.f, 0.f);
	
	//���ݽ� ����Ʈ�� ������ų ��ġ�� �������� ������ ����
	AttackSocket = CreateDefaultSubobject<USceneComponent>(TEXT("AttackSocket"));
	AttackSocket->SetupAttachment(GetMesh());

	/*---------------------------------------------------
								���� UI
	---------------------------------------------------*/

	InfoUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("InfoUI"));
	InfoUI->SetupAttachment(GetMesh());
	InfoUI->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetHelper(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/WBP_ObjectInfo.WBP_ObjectInfo_C'"));
	check(WidgetHelper.Succeeded());
	InfoUI->SetWidgetClass(WidgetHelper.Class);
	InfoUI->SetRelativeLocation(FVector::UpVector * 250.f);
	InfoUI->SetDrawSize(FVector2D(200.f, 40.f));

	/*---------------------------------------------------
								��ƼŬ
	---------------------------------------------------*/
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitReactHelper(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Gruntling/Master/P_ProjectileLob_BreakUp_01.P_ProjectileLob_BreakUp_01'"));
	check(HitReactHelper.Succeeded());
	CreateParticleComponent(CreatureParticleType::HitReact, TEXT("HitReactParticle"), HitReactHelper.Object, FVector::UpVector * 200.f);
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DeathParticleHelper(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Treasure/Resources/P_OrePile_Explosion_CrystalPick.P_OrePile_Explosion_CrystalPick'"));
	check(DeathParticleHelper.Succeeded());
	CreateParticleComponent(CreatureParticleType::Death, TEXT("DeathParticle"), DeathParticleHelper.Object);
}

void ANetworkCreature::CreateParticleComponent(CreatureParticleType _Type, const FName& _Name, UParticleSystem* _Helper, const FVector& _RelativeLocation /*= FVector::ZeroVector*/)
{
	int32 Index = static_cast<int32>(_Type);
	check(nullptr == Particles[Index]);

	UParticleSystemComponent* Particle = CreateDefaultSubobject<UParticleSystemComponent>(_Name);
	Particle->SetupAttachment(GetMesh());
	Particle->SetRelativeLocation(_RelativeLocation);
	Particle->SetTemplate(_Helper);
	Particle->Deactivate();
	Particles[Index] = Particle;
}

void ANetworkCreature::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	InfoUI->InitWidget();
}

void ANetworkCreature::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	ANetworkPlayerController* PlayerCtrl = Cast<ANetworkPlayerController>(GetWorld()->GetFirstPlayerController());
	if (nullptr == PlayerCtrl)
		return;

	ANetworkPlayer* Player = PlayerCtrl->GetPlayer();
	if (nullptr == Player)
		return;

	UCameraComponent* PlayerCam = Player->GetFollowCamera();
	FVector CamLocation = PlayerCam->GetComponentLocation();
	FVector ThisLocation = GetActorLocation();

	double Distance = (CamLocation - ThisLocation).Length();
	bool Visible = (Distance < InfoVisibleLength);
	InfoUI->SetVisibility(Visible);
}




void ANetworkCreature::InitFromType(uint32 _NetworkSecondType)
{
	Super::InitFromType(_NetworkSecondType);

	Protocol::CreatureType CreatureType = static_cast<Protocol::CreatureType>(_NetworkSecondType);
	check(Protocol::CREATURE_TYPE_NONE != CreatureType);
	check(Protocol::CREATURE_TYPE_COUNT != CreatureType);

	USkeletalMeshComponent* SkelMesh = GetMesh();
	SkelMesh->SetSkeletalMesh(AllMeshRes[CreatureType]);
	SkelMesh->SetAnimInstanceClass(AllAnimInstClass[CreatureType]);
	
	//AttackSocketName�̸��� ������ �޽ÿ� �����Ѵٸ�
	if (true == SkelMesh->DoesSocketExist(AttackSocketName))
	{
		//���� �θ�κ��� ����� ���ο� ��ġ�� ����
		AttackSocket->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		AttackSocket->AttachToComponent(SkelMesh, FAttachmentTransformRules::KeepRelativeTransform, AttackSocketName);
		AttackSocket->SetRelativeLocation(FVector::ZeroVector);
	}
	
	//�� ������Ʈ�� ǥ���ϴ� ���� �ʱ�ȭ
	UObjectInfoWidget* InfoWidget = Cast<UObjectInfoWidget>(InfoUI->GetUserWidgetObject());
	check(InfoWidget);
	InfoWidget->Init(this);
}

void ANetworkCreature::SetHp(int32 _Value)
{
	GetProtocolInfo().set_hp(_Value);
	GetInfoUI()->SetHp(static_cast<float>(_Value));
}

UObjectInfoWidget* ANetworkCreature::GetInfoUI()
{
	UObjectInfoWidget* InfoWidget = Cast<UObjectInfoWidget>(InfoUI->GetUserWidgetObject());
	check(InfoWidget);
	return InfoWidget;
}


void ANetworkCreature::PlayParticle(CreatureParticleType _Type)
{
	UParticleSystemComponent* Particle = Particles[static_cast<int32>(_Type)];
	check(Particle);
	Particle->Activate(true);
}


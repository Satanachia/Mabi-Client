// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/StaticActors/FieldWidget/Actor/FieldPortal.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/WidgetComponent.h"
#include "Levels/BattleLevel.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Managers/ResourceManager.h"
#include "Mabinogi.h"

AFieldPortal::AFieldPortal()
{
	/*-------------------------------------------------------------------
											��ƼŬ
	-------------------------------------------------------------------*/
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleHelper(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Ability/Armor/P_Reduced_Melee_Shield_01.P_Reduced_Melee_Shield_01'"));
	check(ParticleHelper.Succeeded());
	UParticleSystem* ParticleSystemAsset = ParticleHelper.Object;
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetTemplate(ParticleSystemAsset);
	ParticleSystemComponent->SetupAttachment(RootComponent);
	
	/*-------------------------------------------------------------------
											����
	-------------------------------------------------------------------*/
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetHelper(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Field/WBP_FieldPortal.WBP_FieldPortal_C'"));
	check(WidgetHelper.Succeeded());
	UWidgetComponent* WidgetComp = GetFieldWidget();
	WidgetComp->SetWidgetClass(WidgetHelper.Class);
	WidgetComp->SetVisibility(false);
}

void AFieldPortal::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	UWidgetComponent* WidgetComp = GetFieldWidget();
	WidgetComp->SetDrawSize(WidgetInitScale);

	UUserWidget* Widget = GetUserWidget();
	check(Widget);
	UTextBlock* WidgetText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("FieldText")));
	check(WidgetText);
	WidgetText->SetText(FText::FromString(WidgetTextValue));
}

void AFieldPortal::Excute(ANetworkPlayerController* _PlayerCtrl)
{
	//���� ������ ExitLevelAreaȣ�� �� �ı�
	ABattleLevel* PrevLevel = Cast<ABattleLevel>(GetWorld()->GetLevelScriptActor());
	ANetworkPlayer* Player = PrevLevel->GetMyPlayer();
	check(NextLevelClass);
	check(PrevLevel);
	check(Player);

	PrevLevel->ExitLevelArea(Player);
	PrevLevel->Destroy();

	//���ο� ���� ���� �� EnterLevelAreaȣ��
	ABattleLevel* NextLevel = GetWorld()->SpawnActor<ABattleLevel>(NextLevelClass);
	NextLevel->AttachToActor(nullptr, FAttachmentTransformRules::KeepRelativeTransform);
	NextLevel->EnterLevelArea(Player, NextLevelStartLocation);
	UMabiGameInstance* GameInst = GET_GAME_INSTANCE(GetWorld());
	GameInst->SetCurrentLevel(NextLevel);

	//���� ��Ȱ��ȭ
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	ResourceManager::PlaySound(SoundType::Portal, GetWorld());
}

void AFieldPortal::OnHoverLMouse(ANetworkPlayerController* _PlayerCtrl)
{
	Super::OnHoverLMouse(_PlayerCtrl);
	UWidgetComponent* WidgetComp = GetFieldWidget();
	WidgetComp->SetVisibility(true);
}

void AFieldPortal::OnUnHoverLMouse(ANetworkPlayerController* _PlayerCtrl)
{
	Super::OnUnHoverLMouse(_PlayerCtrl);
	UWidgetComponent* WidgetComp = GetFieldWidget();
	WidgetComp->SetVisibility(false);
}

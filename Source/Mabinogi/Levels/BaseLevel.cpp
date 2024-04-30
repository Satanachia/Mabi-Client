// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/BaseLevel.h"
#include "Mabinogi.h"
#include "UI/FadeWidget.h"
#include "MabiGameInstance.h"
#include "Actors/Network/NetworkPlayer.h"
#include "Actors/Network/NetworkOthers.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "AnimInst/CreatureAnimInstance.h"
#include "UI/ObjectInfoWidget.h"
#include "Actors/Network/NetworkProjectile.h"
#include "Actors/Network/NetworkItem.h"

/*-------------------------------------------------
							생성자/전역멤버변수 선언
	-------------------------------------------------*/

ABaseLevel::ABaseLevel(Protocol::LevelType _Type)
	:LevelType(_Type)
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UFadeWidget> FadeWidgetHelper(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/WBP_Fade.WBP_Fade_C'"));
	check(FadeWidgetHelper.Succeeded());
	FadeWidgetClass = FadeWidgetHelper.Class;
}


/*-------------------------------------------------
							부모 이벤트 함수
	-------------------------------------------------*/

void ABaseLevel::BeginPlay()
{
	Super::BeginPlay();
	
	check(FadeWidgetClass);
	UUserWidget* Widget = CreateWidget(GetWorld(), FadeWidgetClass);
	check(Widget);
	FadeWidget = Cast<UFadeWidget>(Widget);
	check(FadeWidget);

	FadeWidget->AddToViewport(FADE_WIDGET_LAYER_LEVEL);
	FadeWidget->FadeIn();
	ChangeMainWidget(StartWidgetClass);
	
	//게임 인스턴스에 자신을 현재레벨로 등록
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GetWorld()->GetGameInstance());
	check(GameInst);
	GameInst->SetCurrentLevel(this);
}

void ABaseLevel::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	assert(GameInst);
	GameInst->HandleRecvPackets();
	GameInst->ExcuteLevelJob(this);
}





/*-------------------------------------------------
							위젯 관련
	-------------------------------------------------*/


void ABaseLevel::ChangeMainWidget(TSubclassOf<UUserWidget> _NewWidgetClasss)
{
	if (nullptr != CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	//기존에 위젯이 없던 경우
	if (nullptr != _NewWidgetClasss)
	{
		CurrentWidget = CreateWidget(GetWorld(), _NewWidgetClasss);
		if (nullptr == CurrentWidget)
			return;

		CurrentWidget->AddToViewport();
	}
}

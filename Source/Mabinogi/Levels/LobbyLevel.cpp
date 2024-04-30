// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/LobbyLevel.h"
#include "UI/LobbySelectPlayerWidget.h"
#include "UI/NewPlayerCreateWidget.h"
#include "UI/FadeWidget.h"
#include "Actors/LobbyWatiActor.h"
#include "Kismet/GameplayStatics.h"


ALobbyLevel::ALobbyLevel()
	:ABaseLevel(Protocol::LEVEL_TYPE_NONE)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyLoginWidgetClass (TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/WBP_LobbyLogin.WBP_LobbyLogin_C'"));
	static ConstructorHelpers::FClassFinder<ULobbySelectPlayerWidget> SelectPlayerWidgetClass (TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/WBP_LobbySelectPlayer.WBP_LobbySelectPlayer_C'"));
	static ConstructorHelpers::FClassFinder<UNewPlayerCreateWidget> NewPlayerWidgetClass (TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/WBP_NewPlayerCreate.WBP_NewPlayerCreate_C'"));
	check(LobbyLoginWidgetClass.Succeeded());
	check(SelectPlayerWidgetClass.Succeeded());
	check(NewPlayerWidgetClass.Succeeded());
	SetStartWidgetClass(LobbyLoginWidgetClass.Class);
	SelectPlayer = SelectPlayerWidgetClass.Class;
	NewPlayerCreate = NewPlayerWidgetClass.Class;
}


void ALobbyLevel::ChangeState_NewPlayer(const TFunction<void()>& _CallBack)
{
	UFadeWidget* FadeUI = GetFadeWidget();
	FadeUI->FadeOut([this, _CallBack]()
	{
		ChangeMainWidget(NewPlayerCreate);
		GetFadeWidget()->FadeIn();

		if (nullptr != _CallBack)
			_CallBack();
	});
}

void ALobbyLevel::ChangeState_SelectPlayer(const Protocol::S_LOGIN& _Pkt)
{
	APawn* CurrentPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	assert(CurrentPawn);
	CurrentPawn->AddActorWorldRotation(FRotator(0.f, 180.f, 0.f));

	UWorld* World = GetWorld();
	ALobbyWatiActor* SpawnActor = Cast<ALobbyWatiActor>(World->SpawnActor(ALobbyWatiActor::StaticClass()));
	check(SpawnActor);
	const FVector SpawnPos = CurrentPawn->GetActorLocation() + CurrentPawn->GetActorForwardVector() * 300.f + CurrentPawn->GetActorUpVector() * -100.f;
	SpawnActor->SetActorLocation(SpawnPos);

	UFadeWidget* FadeUI = GetFadeWidget();
	FadeUI->FadeOut([this, SpawnActor, _Pkt]()
	{
		ChangeMainWidget(SelectPlayer);
		GetFadeWidget()->FadeIn();

		ULobbySelectPlayerWidget* Widget = Cast<ULobbySelectPlayerWidget>(GetCurrentWidget());
		check(Widget);
		Widget->Init(_Pkt, SpawnActor);
	});
}

void ALobbyLevel::WaitForExitLobby()
{
	GetFadeWidget()->FadeOut([this]()
	{
		FTimerManager& TimerManager = GetWorldTimerManager();
		TimerManager.SetTimer(TimeHandle, this, &ThisClass::CheckEnterRoom, 0.2f, true);
	});
	ChangeMainWidget(nullptr);
}

void ALobbyLevel::CheckEnterRoom()
{
	if (false == IsReadyEnterRoom)
		return;

	//타이머 종료
	GetWorldTimerManager().ClearTimer(TimeHandle);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("OutDoor"));
}

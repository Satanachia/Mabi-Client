// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbySelectPlayerWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Actors/LobbyWatiActor.h"
#include "Mabinogi.h"
#include "GameFramework/GameModeBase.h"
#include "Levels/LobbyLevel.h"

void ULobbySelectPlayerWidget::Init(const Protocol::S_LOGIN& _Pkt, ALobbyWatiActor* _Actor)
{
	PlayerInfoes = _Pkt;
	PreviewActor = _Actor;
	check(PreviewActor);
	
	SettingInfo(Cursor);
}

void ULobbySelectPlayerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	LeftButton = Cast<UButton>(GetWidgetFromName(TEXT("LeftButton")));
	RightButton = Cast<UButton>(GetWidgetFromName(TEXT("RightButton")));
	SelectButton = Cast<UButton>(GetWidgetFromName(TEXT("SelectButton")));
	NewPlayerButton = Cast<UButton>(GetWidgetFromName(TEXT("NewPlayerButton")));
	NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));

	check(LeftButton);
	check(RightButton);
	check(SelectButton);
	check(NewPlayerButton);
	check(NameText);
	
	LeftButton->OnClicked.AddDynamic(this, &ThisClass::LBtnCallback);
	RightButton->OnClicked.AddDynamic(this, &ThisClass::RBtnCallback);
	SelectButton->OnClicked.AddDynamic(this, &ThisClass::SelectCallBack);
	NewPlayerButton->OnClicked.AddDynamic(this, &ThisClass::ChangeToNewPlayerWidget);
}

void ULobbySelectPlayerWidget::SettingInfo(int32 _Index)
{
	Cursor = _Index;

	if (PlayerInfoes.players_size() <= Cursor)
		Cursor = 0;
	else if (Cursor < 0)
		Cursor = PlayerInfoes.players_size() - 1;
	
	const Protocol::ObjectInfo& Info = PlayerInfoes.players(Cursor);
	PreviewActor->ChangeMesh(static_cast<Protocol::CreatureType>(Info.object_id()));
	NameText->SetText(FText::FromString(FString(Info.object_name().data())));
}

void ULobbySelectPlayerWidget::SelectCallBack()
{
	LeftButton->SetIsEnabled(false);
	RightButton->SetIsEnabled(false);
	SelectButton->SetIsEnabled(false);
	NewPlayerButton->SetIsEnabled(false);

	Protocol::C_ENTER_GAME enterPkt;
	enterPkt.set_playerindex(Cursor);
	SEND_PACKET(enterPkt);

	ALobbyLevel* Level = Cast<ALobbyLevel>(GetWorld()->GetLevelScriptActor());
	check(Level);
	Level->WaitForExitLobby();
}

void ULobbySelectPlayerWidget::ChangeToNewPlayerWidget()
{
	UWorld* World = GetWorld();
	ALobbyLevel* Level = Cast<ALobbyLevel>(World->GetLevelScriptActor());
	check(Level);
	Level->ChangeState_NewPlayer([World]()
	{
		APawn* CurrentPawn = World->GetFirstPlayerController()->GetPawn();
		check(CurrentPawn);
		CurrentPawn->AddActorWorldRotation(FRotator(0.f, 180.f, 0.f));
	});
}

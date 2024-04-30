// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NewPlayerCreateWidget.h"
#include "Components/EditableTextBox.h"
#include "Actors/LobbyActorBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "EngineUtils.h"
#include "Mabinogi.h"
#include "Levels/LobbyLevel.h"
#include "GameFramework/GameModeBase.h"


void UNewPlayerCreateWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	/*---------------------------------------------------------------------------------
													UI ���ε�
	---------------------------------------------------------------------------------*/

	Buttons[BUTTON_TYPE_SELECT_MAGE] = Cast<UButton>(GetWidgetFromName(TEXT("SelectMageButton")));
	Buttons[BUTTON_TYPE_SELECT_WARROR] = Cast<UButton>(GetWidgetFromName(TEXT("SelectWarriorButton")));
	Buttons[BUTTON_TYPE_SELECT_ROG] = Cast<UButton>(GetWidgetFromName(TEXT("SelectRogButtom")));
	Buttons[BUTTON_TYPE_OK] = Cast<UButton>(GetWidgetFromName(TEXT("OkButton")));
	Buttons[BUTTON_TYPE_BACK] = Cast<UButton>(GetWidgetFromName(TEXT("BackButton")));
	for (UButton* Button : Buttons)
		check(Button);

	JobName = Cast<UTextBlock>(GetWidgetFromName(TEXT("JobNameText")));
	check(JobName);
	
	//Hover�ݹ� ���
	Buttons[BUTTON_TYPE_SELECT_MAGE]->OnHovered.AddDynamic(this, &UNewPlayerCreateWidget::HoverMage);
	Buttons[BUTTON_TYPE_SELECT_WARROR]->OnHovered.AddDynamic(this, &UNewPlayerCreateWidget::HoverWarrior);
	Buttons[BUTTON_TYPE_SELECT_ROG]->OnHovered.AddDynamic(this, &UNewPlayerCreateWidget::HoverRog);

	//Ŭ�� �ݹ� ���
	Buttons[BUTTON_TYPE_SELECT_MAGE]->OnClicked.AddDynamic(this, &UNewPlayerCreateWidget::SelectMage);
	Buttons[BUTTON_TYPE_SELECT_WARROR]->OnClicked.AddDynamic(this, &UNewPlayerCreateWidget::SelectWarrior);
	Buttons[BUTTON_TYPE_SELECT_ROG]->OnClicked.AddDynamic(this, &UNewPlayerCreateWidget::SelectRog);
	Buttons[BUTTON_TYPE_OK]->OnClicked.AddDynamic(this, &UNewPlayerCreateWidget::ExcuteOkButton);
	Buttons[BUTTON_TYPE_BACK]->OnClicked.AddDynamic(this, &UNewPlayerCreateWidget::ExcuteBackButton);

	NameText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("InputName")));
	check(NameText);

	Buttons[BUTTON_TYPE_OK]->SetIsEnabled(false);
	Buttons[BUTTON_TYPE_BACK]->SetIsEnabled(false);

	/*---------------------------------------------------------------------------------
													������ �����ϴ� ���� ����
	---------------------------------------------------------------------------------*/

	int32 LabelEraseStartPos = FString(TEXT("BP_Lobby")).Len();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALobbyActorBase::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		FString Name = Actor->GetActorLabel().Mid(LabelEraseStartPos);
		Protocol::CreatureType Type = Protocol::CREATURE_TYPE_NONE;

		if (TEXT("Mage") == Name)
			Type = Protocol::CREATURE_TYPE_PLAYER_MAGE;

		else if (TEXT("Warrior") == Name)
			Type = Protocol::CREATURE_TYPE_PLAYER_WARRIOR;

		else if (TEXT("Rog") == Name)
			Type = Protocol::CREATURE_TYPE_PLAYER_ROG;

		check(Protocol::CREATURE_TYPE_NONE != Type);
		ALobbyActorBase* LobbyActor = Cast<ALobbyActorBase>(Actor);
		check(LobbyActor);

		LobbyActor->SetLobbyName(Name);
		Actors.Add(Type, LobbyActor);
	}
}



/*---------------------------------------------------------------------------------
												UI �ݹ� - ĳ���� ȣ��/Ŭ��
---------------------------------------------------------------------------------*/

void UNewPlayerCreateWidget::SetHoverState(Protocol::CreatureType  _Type)
{
	if (Protocol::CREATURE_TYPE_NONE != SelectedType)
		return;
	
	for (const TPair<Protocol::CreatureType, ALobbyActorBase*>& Pair : Actors)
	{
		Protocol::CreatureType Type = Pair.Key;
		ALobbyActorBase* Actor = Pair.Value;
		
		if (Type != _Type)
		{
			Actor->SetAniState(LobbyActorState::Idle);
			continue;
		}
		
		Actor->SetAniState(LobbyActorState::Hover);
		JobName->SetText(FText::FromString(Actor->GetLobbyName()));
	}
}

void UNewPlayerCreateWidget::SetSelectState(Protocol::CreatureType _Type)
{
	if (Protocol::CREATURE_TYPE_NONE != SelectedType)
		return;
	
	for (const TPair<Protocol::CreatureType, ALobbyActorBase*>& Pair : Actors)
	{
		Protocol::CreatureType Type = Pair.Key;
		ALobbyActorBase* Actor = Pair.Value;

		if (Type == _Type)
		{
			Actor->SetAniState(LobbyActorState::Selected);
		}
		else
		{
			Actor->SetAniState(LobbyActorState::Idle);
		}
	}
	
	SelectedType = _Type;
	Buttons[BUTTON_TYPE_OK]->SetIsEnabled(true);
	Buttons[BUTTON_TYPE_BACK]->SetIsEnabled(true);
}




/*---------------------------------------------------------------------------------
												UI �ݹ� - ��ư
---------------------------------------------------------------------------------*/

void UNewPlayerCreateWidget::ExcuteOkButton()
{
	//ĳ���͸� ������ �� ���ٸ� return
	if (Protocol::CREATURE_TYPE_NONE == SelectedType)
		return;

	//������ ĳ������ Ÿ���� �÷��̾� Ÿ������ Ȯ��
	bool IsPlayerTypeCheck = false;
	IsPlayerTypeCheck |= (Protocol::CREATURE_TYPE_PLAYER_MAGE == SelectedType);
	IsPlayerTypeCheck |= (Protocol::CREATURE_TYPE_PLAYER_WARRIOR == SelectedType);
	IsPlayerTypeCheck |= (Protocol::CREATURE_TYPE_PLAYER_ROG == SelectedType);
	check(IsPlayerTypeCheck);
	
	
	//�� ĳ���� ���� ��Ŷ ����
	Protocol::C_CREATE_NEW_PLAYER Pkt;
	FString InputName = NameText->GetText().ToString();
	Pkt.set_player_name(TCHAR_TO_ANSI(*InputName));
	Pkt.set_player_type(SelectedType);
	SEND_PACKET(Pkt);

	
	//������ ��ȯ�Ǳ� ������ ���
	ALobbyLevel* Level = Cast<ALobbyLevel>(GWorld->GetLevelScriptActor());
	check(Level);
	Level->WaitForExitLobby();
}

void UNewPlayerCreateWidget::ExcuteBackButton()
{
	//ĳ���͸� ������ �� ���ٸ� return
	if (Protocol::CREATURE_TYPE_NONE == SelectedType)
		return;
	
	//ĳ���� ���� ��� �۾�
	Buttons[BUTTON_TYPE_OK]->SetIsEnabled(false);
	Buttons[BUTTON_TYPE_BACK]->SetIsEnabled(false);
	SelectedType = Protocol::CREATURE_TYPE_NONE;
	
	//��� ĳ���� Idle���·� ������
	for (const TPair<Protocol::CreatureType, ALobbyActorBase*>& Pair : Actors)
	{
		ALobbyActorBase* Actor = Pair.Value;
		Actor->SetAniState(LobbyActorState::Idle);
	}
}

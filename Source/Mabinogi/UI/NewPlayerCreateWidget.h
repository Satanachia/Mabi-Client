// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protocol.pb.h"
#include "NewPlayerCreateWidget.generated.h"

/**
 * 
 */
UCLASS()
class MABINOGI_API UNewPlayerCreateWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void HoverMage() { SetHoverState(Protocol::CREATURE_TYPE_PLAYER_MAGE); }
	UFUNCTION()
	void HoverWarrior() { SetHoverState(Protocol::CREATURE_TYPE_PLAYER_WARRIOR); }
	UFUNCTION()
	void HoverRog() { SetHoverState(Protocol::CREATURE_TYPE_PLAYER_ROG); }
	UFUNCTION()
	void SelectMage() { SetSelectState(Protocol::CREATURE_TYPE_PLAYER_MAGE); }
	UFUNCTION()
	void SelectWarrior() { SetSelectState(Protocol::CREATURE_TYPE_PLAYER_WARRIOR); }
	UFUNCTION()
	void SelectRog() { SetSelectState(Protocol::CREATURE_TYPE_PLAYER_ROG); }
	
	void SetHoverState(Protocol::CreatureType _Type);
	void SetSelectState(Protocol::CreatureType _Type);

	UFUNCTION()
	void ExcuteOkButton();
	UFUNCTION()
	void ExcuteBackButton();

private:
	enum ButtonType
	{
		BUTTON_TYPE_SELECT_MAGE = 0,
		BUTTON_TYPE_SELECT_WARROR = 1,
		BUTTON_TYPE_SELECT_ROG = 2,
		BUTTON_TYPE_OK = 3,
		BUTTON_TYPE_BACK = 4,
		BUTTON_TYPE_COUNT = 5
	};

	class UButton* Buttons[BUTTON_TYPE_COUNT];
	TMap<Protocol::CreatureType, class ALobbyActorBase*> Actors;
	class UEditableTextBox* NameText;
	class UTextBlock* JobName;

	Protocol::CreatureType SelectedType = Protocol::CREATURE_TYPE_NONE;
};

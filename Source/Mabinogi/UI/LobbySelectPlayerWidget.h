// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protocol.pb.h"
#include "LobbySelectPlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class MABINOGI_API ULobbySelectPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(const Protocol::S_LOGIN& _Pkt, class ALobbyWatiActor* _Actor);

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void LBtnCallback() { SettingInfo(Cursor - 1); }
	UFUNCTION()
	void RBtnCallback() { SettingInfo(Cursor + 1); }
	
	void SettingInfo(int32 _Index);

	UFUNCTION()
	void SelectCallBack();

	UFUNCTION()
	void ChangeToNewPlayerWidget();

private:
	class UButton* LeftButton;
	class UButton* RightButton;
	class UButton* SelectButton;
	class UButton* NewPlayerButton;
	class UTextBlock* NameText;

	/*
		S_LOGIN패킷의 object_id에는 실제 오브젝트 아이디가 존재하지 않고
		접속한 유저의 캐릭터들의 직업이 들어있음
	*/
	int32 Cursor = 0;
	Protocol::S_LOGIN PlayerInfoes;
	class ALobbyWatiActor* PreviewActor = nullptr;
};

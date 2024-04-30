// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "MabiGameInstance.h"
#include "Components/TextBlock.h"
#include "Mabinogi.h"


void ULobbyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	LoginButtom = Cast<UButton>(GetWidgetFromName(TEXT("LogInBtn")));
	SignupButtom = Cast<UButton>(GetWidgetFromName(TEXT("SignUpBtn")));
	IpTextBlock = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("IpInput")));
	IdTextBlock = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("IdInput")));
	PwTextBlock = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("PwInput")));
	ResText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ResultText")));

	if (nullptr == LoginButtom ||
		nullptr == SignupButtom||
		nullptr == IpTextBlock ||
		nullptr == IdTextBlock ||
		nullptr == PwTextBlock||
		nullptr == ResText)
	{
		//로그 출력
		SCREEN_ERROR_MSG("Failed Lobby Resource");
		return;
	}
	
	ResText->SetText(FText::FromString(TEXT(" ")));
	LoginButtom->OnClicked.AddDynamic(this, &ULobbyWidget::ExcuteLogin);
	SignupButtom->OnClicked.AddDynamic(this, &ULobbyWidget::ExcuteSignup);
}

void ULobbyWidget::BtnExcute(bool _IsSignUp)
{
	LoginButtom->SetIsEnabled(false);
	SignupButtom->SetIsEnabled(false);

	//서버에 접속
	FString Ip = IpTextBlock->GetText().ToString();
	UMabiGameInstance* GameInstance = Cast<UMabiGameInstance>(GetWorld()->GetGameInstance());
	ResText->SetText(FText::FromString(TEXT("Connecting to server...")));
	bool Result = GameInstance->ConnectToGameServer(Ip);
	
	//접속에 실패한 경우
	if (false == Result)
	{
		LoginButtom->SetIsEnabled(true);
		SignupButtom->SetIsEnabled(true);
		ResText->SetText(FText::FromString(TEXT("Wrong IP Address")));
		return;
	}

	//서버에 접속
	FString Id = IdTextBlock->GetText().ToString();
	FString Pw = PwTextBlock->GetText().ToString();

	//로그인 요청 패킷 보내기
	Protocol::C_LOGIN LoginPkt;
	LoginPkt.set_login_id(TCHAR_TO_ANSI(*Id));
	LoginPkt.set_login_pw(TCHAR_TO_ANSI(*Pw));
	LoginPkt.set_sign_up(_IsSignUp);
	SEND_PACKET(LoginPkt);
}


void ULobbyWidget::FailedLogin()
{
	LoginButtom->SetIsEnabled(true);
	SignupButtom->SetIsEnabled(true);
	ResText->SetText(FText::FromString(TEXT("Non-Existent Account or Duplication ID")));
}
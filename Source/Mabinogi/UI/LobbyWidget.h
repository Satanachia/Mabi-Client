// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class MABINOGI_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void FailedLogin();
	
protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void ExcuteLogin() { BtnExcute(false); }

	UFUNCTION()
	void ExcuteSignup() { BtnExcute(true); }

	void BtnExcute(bool _IsSignUp);
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* LoginButtom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* SignupButtom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UEditableTextBox* IpTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UEditableTextBox* IdTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UEditableTextBox* PwTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* ResText;

	
};

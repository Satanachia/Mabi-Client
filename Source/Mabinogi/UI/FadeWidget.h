// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FadeWidget.generated.h"


UENUM(BlueprintType)
enum class EFadeState : uint8
{
	Wait UMETA(DisplayName = "Wait"),
	FadeIn UMETA(DisplayName = "FadeIn"),
	FadeOut UMETA(DisplayName = "FadeOut"),
};

DECLARE_DELEGATE(FFadeOutEvent)
DECLARE_DELEGATE(FFadeInEvent)

/**
 * 
 */
UCLASS()
class MABINOGI_API UFadeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void FadeOut(TFunction<void()>&& _FadeInEndCallBack = nullptr, float _Duration = 0.5f, FLinearColor _StartColor = FLinearColor::Black);
	void FadeIn(TFunction<void()>&& _FadeOutEndCallBack = nullptr, float _Duration = 0.5f, FLinearColor _StartColor = FLinearColor::Black);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& _Geometry, float _DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UImage* Fade;

	EFadeState State = EFadeState::Wait;
	FLinearColor CurColor;
	float FadeDuration = 0.f;
	float CurTime = 0.f;

	TFunction<void()> FadeInEndCallBack;
	TFunction<void()> FadeOutEndCallBack;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FadeWidget.h"
#include "Mabinogi.h"
#include "Components/Image.h"


void UFadeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Fade = Cast<UImage>(GetWidgetFromName(TEXT("FadeImage")));
	if (nullptr == Fade)
	{
		SCREEN_ERROR_MSG("Failed Lobby Fade Resource");
		return;
	}

	Fade->SetColorAndOpacity(FLinearColor::Transparent);
}


void UFadeWidget::FadeIn(
	TFunction<void()>&& _FadeInEndCallBack /*= nullptr*/,
	float _Duration /*= 0.5f*/, 
	FLinearColor _StartColor /*= FLinearColor::Black*/)
{
	CurColor = _StartColor;
	CurColor.A = 1.f;
	Fade->SetColorAndOpacity(CurColor);

	FadeDuration = _Duration;
	CurTime = 0.f;
	State = EFadeState::FadeIn;

	FadeInEndCallBack = std::move(_FadeInEndCallBack);
}

void UFadeWidget::FadeOut(
	TFunction<void()>&& _FadeOutEndCallBack /*= nullptr*/,
	float _Duration /*= 0.5f*/, 
	FLinearColor _StartColor /*= FLinearColor::Black*/)
{
	CurColor = _StartColor;
	CurColor.A = 0.f;
	Fade->SetColorAndOpacity(CurColor);

	FadeDuration = _Duration;
	CurTime = 0.f;
	State = EFadeState::FadeOut;

	FadeOutEndCallBack = std::move(_FadeOutEndCallBack);
}


void UFadeWidget::NativeTick(const FGeometry& _Geometry, float _DeltaTime)
{
	Super::NativeTick(_Geometry, _DeltaTime);

	if (EFadeState::Wait == State)
		return;

	CurTime += _DeltaTime;
	float Ratio = FMath::Clamp(CurTime / FadeDuration, 0.f, 1.f);
	CurColor.A = (EFadeState::FadeIn == State) ? (1.f - Ratio) : Ratio;;
	Fade->SetColorAndOpacity(CurColor);

	if (Ratio < 1.f)
		return;

	EFadeState PrevState = State;
	State = EFadeState::Wait;

	if (EFadeState::FadeIn == PrevState)
	{
		if(nullptr != FadeInEndCallBack)
			FadeInEndCallBack();
		FadeInEndCallBack = nullptr;
	}
	else
	{
		if (nullptr != FadeOutEndCallBack)
			FadeOutEndCallBack();
		FadeOutEndCallBack = nullptr;
	}
}

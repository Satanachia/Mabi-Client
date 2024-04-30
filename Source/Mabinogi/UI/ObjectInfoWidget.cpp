// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ObjectInfoWidget.h"
#include "Components/TextBlock.h"
#include "Actors/Network/NetworkObject.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "MathUtil.h"
#include "Managers/ResourceManager.h"


void UObjectInfoWidget::Init(ANetworkObject* _Owner)
{
	NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ObjectName_TextBlock")));
	check(NameText);
	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Hp_Bar")));
	check(HpBar);
	ThinkingImg = Cast<UImage>(GetWidgetFromName(TEXT("Thinking")));
	check(ThinkingImg);
	ActionImg = Cast<UImage>(GetWidgetFromName(TEXT("Action")));
	check(ActionImg);

	const std::string OwnerName = _Owner->GetProtocolName();
	NameText->SetText(FText::FromString(FString(OwnerName.data())));

	ThinkingImg->SetOpacity(0.f);
	ActionImg->SetOpacity(0.f);
}

void UObjectInfoWidget::SetHp(float _Value)
{
	float Ratio = _Value / static_cast<float>(MAX_HP);
	Ratio = std::clamp(Ratio, 0.f, 1.f);
	HpBar->SetPercent(Ratio);
}

void UObjectInfoWidget::StartThink(Protocol::AttackType _AttackType, UTexture2D* _Texture)
{
	check(_Texture);

	ActionImg->SetBrushFromTexture(_Texture);
	ActionTimer = 0.f;
	CurState = State::E_ReadyAction;
	CurrentReadyThink = _AttackType;
}

void UObjectInfoWidget::StartThink(Protocol::AttackType _AttackType)
{
	UTexture2D* LoadedTexture = ResourceManager::GetTexture(_AttackType);
	StartThink(_AttackType, LoadedTexture);
}

void UObjectInfoWidget::FinishThink(Protocol::AttackType _AttackType, bool _IsOk /*= true*/)
{
	if (CurrentReadyThink != _AttackType || false == _IsOk)
	{
		ThinkingImg->SetOpacity(0.f);
		ActionImg->SetOpacity(0.f);
		CurrentReadyThink = Protocol::ATTACK_TYPE_NONE;
		CurState = State::E_Idle;
	}
	else
	{
		ThinkingImg->SetOpacity(1.f);
		ActionImg->SetOpacity(1.f);
		CurState = State::E_CompleteAction;
	}
}

void UObjectInfoWidget::FadeOut()
{
	ThinkingImg->SetOpacity(1.f);
	ActionImg->SetOpacity(1.f);
	CurState = State::E_FadeOut;
	ActionTimer = 0.f;
}


void UObjectInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	switch (CurState)
	{
	case UObjectInfoWidget::E_ReadyAction:
		Update_ReadyAction(InDeltaTime);
		break;
	case UObjectInfoWidget::E_FadeOut:
		Update_FadeOut(InDeltaTime);
		break;
	}
}

void UObjectInfoWidget::Update_ReadyAction(float _DeltaTime)
{
	ActionTimer += _DeltaTime;
	float SinValue = FMath::Sin(2.f * PI * ActionTimer);
	SinValue = FMath::Abs(SinValue);

	ThinkingImg->SetOpacity(SinValue);
	ActionImg->SetOpacity(SinValue);
}

void UObjectInfoWidget::Update_FadeOut(float _DeltaTime)
{
	const float Duration = 1.f;

	ActionTimer += _DeltaTime;
	float Ratio = FMath::Clamp(ActionTimer / Duration, 0.f, 1.f);
	Ratio = 1.f - Ratio;
	ThinkingImg->SetOpacity(Ratio);
	ActionImg->SetOpacity(Ratio);

	if (ActionTimer < Duration)
		return;
	
	CurState = UObjectInfoWidget::E_Idle;
	ThinkingImg->SetOpacity(0.f);
	ActionImg->SetOpacity(0.f);
	CurrentReadyThink = Protocol::ATTACK_TYPE_NONE;
}

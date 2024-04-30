// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ActionSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include"UI/HUD/HUDWidget.h"


void UActionSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ActionSlotName")));
	check(NameText);
}



void UActionSlotWidget::InitSlotNumber(int32 _Num)
{
	static const FString SlotName = TEXT("Slot ");
	FString TextValue = SlotName + FString::FromInt(_Num);
	NameText->SetText(FText::FromString(TextValue));
}





void UActionSlotWidget::ExcuteAction()
{
	if (true == IsEmpty())
		return;
	
	Target->ExcuteAction();
}

void UActionSlotWidget::LeaveSlot(USlotWidget* _MovingSlot)
{
	Super::LeaveSlot(_MovingSlot);
	GetSlotImg()->SetOpacity(0.5f);
}

void UActionSlotWidget::EnterSlot(USlotWidget* _MovingSlot)
{
	Super::EnterSlot(_MovingSlot);

	Copy(_MovingSlot);

	//슬롯내 중복 제거
	UActionSlotWidget* OverlabSlot = GetHud()->FindActionSlot(Target, this);
	if (nullptr == OverlabSlot)
		return;
	
	OverlabSlot->Copy(nullptr);
}

bool UActionSlotWidget::IsEmpty()
{
	bool Result = Super::IsEmpty();
	Result &= (nullptr == Target);
	return Result;
}

void UActionSlotWidget::CancelMove()
{
	Super::CancelMove();

	//슬롯에서 제거
	Copy(nullptr);
}

void UActionSlotWidget::Copy(USlotWidget* _Target)
{
	UTexture2D* Texture = nullptr;
	float TextureAlpha = 0.f;
	int32 CountValue = 0;

	if (nullptr != _Target)
	{
		Texture = _Target->GetTexture();
		CountValue = _Target->GetCountValue();
		TextureAlpha = (nullptr == Texture) ? 0.f : 1.f;
	}
	
	SetTexture(Texture, TextureAlpha);
	SetCountValue(CountValue);
	Target = _Target;

	//인자로 들어온 USlotWidget가 ActionSlot인 경우
	UActionSlotWidget* TargetActionSlot = Cast<UActionSlotWidget>(Target);
	if (nullptr == TargetActionSlot)
		return;
	
	//인자인 ActionSlot의 타겟을 내 타겟으로 지정
	Target = TargetActionSlot->Target;
	check(nullptr == Cast<UActionSlotWidget>(Target));
}


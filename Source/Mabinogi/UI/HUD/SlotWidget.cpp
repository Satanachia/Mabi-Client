// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SlotWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include"Components/TextBlock.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "UI/MouseCursorWidget.h"
#include "Levels/BattleLevel.h"
#include "Mabinogi.h"

void USlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SlotImg = Cast<UImage>(GetWidgetFromName(TEXT("SlotImage")));
	SlotBtn = Cast<UButton>(GetWidgetFromName(TEXT("SlotButton")));
	CountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CountText")));
	check(SlotImg);
	check(SlotBtn);
	check(CountText);
	SlotBtn->OnClicked.AddDynamic(this, &ThisClass::OnMouseClickCallBack);
	SlotImg->SetOpacity(0.f);
}


void USlotWidget::OnMouseClickCallBack()
{
	ANetworkPlayerController* PlayerCtrl = Cast<ANetworkPlayerController>(GetWorld()->GetFirstPlayerController());
	check(PlayerCtrl);
	UMouseCursorWidget* MouseCursorWidget = PlayerCtrl->GetCursorWidget();
	USlotWidget* MouseMovingSlot = MouseCursorWidget->GetMovingSlot();
	
	//이 슬롯에 있는 액션이 다른곳으로 이사가기
	if (false == IsEmpty() && nullptr == MouseMovingSlot)
	{
		MouseCursorWidget->ChangeCursorTexture(this);
		LeaveSlot(MouseMovingSlot);
		return;
	}
	
	//아무일도 일어나지 않음
	if (nullptr == MouseMovingSlot)
		return;

	//마우스 커서에 있는 액션이 이 슬롯으로 이동 시도
	EnterSlot(MouseMovingSlot);
	CheckOpacity();
	MouseMovingSlot->CheckOpacity();
	
	//커서는 원래 상태로 되돌리기
	MouseCursorWidget->ChangeDefaultCursor();
}

void USlotWidget::CancelMove()
{
	CheckOpacity();
}

void USlotWidget::CheckOpacity()
{
	float Alpha = IsEmpty() ? 0.f : 1.f;
	SlotImg->SetOpacity(Alpha);
}


bool USlotWidget::IsEmpty()
{
	return (nullptr == GetTexture());
}



const FVector2D& USlotWidget::GetImageScale()
{
	return SlotImg->GetCachedGeometry().GetLocalSize();
}



UTexture2D* USlotWidget::GetTexture()
{
	return Cast<UTexture2D>(SlotImg->Brush.GetResourceObject());
}

void USlotWidget::SetTexture(UTexture2D* _Tex, float _Alpha /*= 1.f*/)
{
	SlotImg->SetBrushFromTexture(_Tex);
	SlotImg->SetOpacity(_Alpha);
}

int32 USlotWidget::GetCountValue()
{
	FText CountStr =  CountText->GetText();
	if (true == CountStr.IsEmpty())
	{
		return -1;
	}
	
	int32 Result = FCString::Atoi(*CountStr.ToString());
	return Result;
}


void USlotWidget::SetCountValue(int32 _Value)
{
	FString ValueStr;
	FText Text;
	if (_Value <= 0)
	{
		ValueStr = TEXT("");
	}
	else
	{
		ValueStr = FString::FromInt(_Value);
	}
	
	CountText->SetText(FText::FromString(ValueStr));
}

UHUDWidget* USlotWidget::GetHud()
{
	UMabiGameInstance* GameInst = GET_GAME_INSTANCE(GetWorld());
	check(GameInst);
	ABattleLevel* Level = Cast<ABattleLevel>(GameInst->GetCurrentLevel());
	check(Level);
	UHUDWidget* HUD = Level->GetHUDWidget();
	check(HUD);
	return HUD;
}



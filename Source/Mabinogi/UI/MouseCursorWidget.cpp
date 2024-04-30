// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MouseCursorWidget.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "UI/HUD/SlotWidget.h"

void UMouseCursorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CursorImage = Cast<UImage>(GetWidgetFromName(TEXT("CursorImage")));
	check(CursorImage);
}


void UMouseCursorWidget::ChangeCursorTexture(USlotWidget* _Slot)
{
	check(_Slot);

	MovingSlot = _Slot;
	UTexture2D* Texture = _Slot->GetTexture();
	const FVector2D& ImgScale = _Slot->GetImageScale();

	ChangeCursorTexture(Texture, ImgScale, 0.5f);
}

void UMouseCursorWidget::ChangeCursorTexture(UTexture2D* _Tex, const FVector2D& _Scale, float _Alpha /*= 1.f*/)
{
	CheckDefaultTexture();

	CursorImage->SetBrushFromTexture(_Tex);
	CursorImage->SetDesiredSizeOverride(_Scale);
	CursorImage->SetOpacity(_Alpha);
}

void UMouseCursorWidget::CheckDefaultTexture()
{
	if (nullptr != DefaultTexture)
		return;

	check(CursorImage);
	DefaultTexture = Cast<UTexture2D>(CursorImage->Brush.GetResourceObject());
	DefaultScale = CursorImage->GetCachedGeometry().GetLocalSize();
	check(DefaultTexture);
}


void UMouseCursorWidget::ChangeDefaultCursor()
{
	if (nullptr == MovingSlot)
		return;

	check(DefaultTexture);
	ChangeCursorTexture(DefaultTexture, DefaultScale);
	MovingSlot = nullptr;
}

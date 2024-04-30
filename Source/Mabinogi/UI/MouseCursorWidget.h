// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MouseCursorWidget.generated.h"

class UImage;
class UTexture2D;
class USlotWidget;

/**
 * 
 */
UCLASS()
class MABINOGI_API UMouseCursorWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void ChangeCursorTexture(USlotWidget* _Slot);
	void ChangeDefaultCursor();

	USlotWidget* GetMovingSlot() { return MovingSlot; }

private:
	void ChangeCursorTexture(UTexture2D* _Tex, const FVector2D& _Scale, float _Alpha = 1.f);
	void CheckDefaultTexture();

private:
	UImage* CursorImage = nullptr;
	UTexture2D* DefaultTexture = nullptr;
	FVector2D DefaultScale;

	USlotWidget* MovingSlot = nullptr;
};

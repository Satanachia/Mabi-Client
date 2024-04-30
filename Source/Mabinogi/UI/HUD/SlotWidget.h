// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protocol.pb.h"
#include "SlotWidget.generated.h"

class UImage;
class UButton;
class UTextBlock;


/**
 * 
 */
UCLASS()
class MABINOGI_API USlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnMouseClickCallBack();
	
protected:
	virtual void LeaveSlot(USlotWidget* _MovingSlot){}
	virtual void EnterSlot(USlotWidget* _MovingSlot){}
	virtual bool IsEmpty();
	
public:
	virtual void ExcuteAction(){}
	virtual void CancelMove();
	
public:
	UTexture2D* GetTexture();
	const FVector2D& GetImageScale();
	int32 GetCountValue();


protected:
	UImage* GetSlotImg() { return SlotImg; }
	UButton* GetSlotBtn() { return SlotBtn; }
	void SetTexture(UTexture2D* _Tex, float _Alpha = 1.f);
	void SetCountValue(int32 _Value);

	class UHUDWidget* GetHud();

private:
	void CheckOpacity();

private:
	UImage* SlotImg;
	UButton* SlotBtn;
	UTextBlock* CountText;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protocol.pb.h"
#include "ObjectInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class MABINOGI_API UObjectInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
	enum State
	{
		E_Idle,
		E_ReadyAction,		//스킬 준비중
		E_CompleteAction,	//스킬 준비 완료
		E_FadeOut,				//사용 취소 or 사용 완료
	};

public:
	void Init(class ANetworkObject* _Owner);
	void SetHp(float _Value);
	
	void StartThink(Protocol::AttackType _AttackType, class UTexture2D* _Texture);
	void StartThink(Protocol::AttackType _AttackType);
	void FinishThink(Protocol::AttackType _AttackType, bool _IsOk = true);
	
	bool IsNoThink() { return Protocol::ATTACK_TYPE_NONE == CurrentReadyThink; }
	void FadeOut();

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
		
private:
	void Update_ReadyAction(float _DeltaTime);
	void Update_FadeOut(float _DeltaTime);

private:
	enum { MAX_HP = 100 };

	class UTextBlock* NameText;
	class UProgressBar* HpBar;
	class UImage* ThinkingImg;
	class UImage* ActionImg;
	
	State CurState = State::E_Idle;
	float ActionTimer = 0.f;
	Protocol::AttackType CurrentReadyThink = Protocol::ATTACK_TYPE_NONE;
};

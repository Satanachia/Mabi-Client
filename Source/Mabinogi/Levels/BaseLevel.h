// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Protocol.pb.h"
#include "BaseLevel.generated.h"

class UFadeWidget;
class APawn;
class UUserWidget;
class UMabiGameInstance;
class ANetworkPlayer;
class ANetworkOthers;
class ANetworkObject;


/**
 * 
 */
UCLASS()
class MABINOGI_API ABaseLevel : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	ABaseLevel() { assert(false); }
	ABaseLevel(Protocol::LevelType _Type);
	
protected:
	/*-------------------------------------------------
											부모쪽 이벤트 함수
	-------------------------------------------------*/
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	
public:
	/*-------------------------------------------------
											위젯 관련
	-------------------------------------------------*/

	void ChangeMainWidget(TSubclassOf<UUserWidget> _NewWidgetClasss);
	UUserWidget* GetCurrentWidget() { return CurrentWidget; }

protected:
	UFadeWidget* GetFadeWidget() { return FadeWidget; }
	void SetStartWidgetClass(TSubclassOf<UUserWidget> _StartWidgetClass) { StartWidgetClass = _StartWidgetClass; }

private:
	enum { FADE_WIDGET_LAYER_LEVEL = 10 };

	TSubclassOf<UFadeWidget> FadeWidgetClass;
	TSubclassOf<UUserWidget> StartWidgetClass;
	UFadeWidget* FadeWidget;
	UUserWidget* CurrentWidget = nullptr;

public:
	/*-------------------------------------------------
										JobQueue 관련
	-------------------------------------------------*/
	
	Protocol::LevelType GetLevelType() { return LevelType; }

private:
	Protocol::LevelType LevelType = Protocol::LEVEL_TYPE_NONE;
	
	float Timer = 0.f;
	int32 FrameCount = 0;
};

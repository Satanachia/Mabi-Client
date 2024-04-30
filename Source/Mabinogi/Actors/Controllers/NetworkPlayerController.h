// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Protocol.pb.h"
#include "FSM/PlayerFSM.h"
#include "Define.h"
#include "NetworkPlayerController.generated.h"

class UStateBase;
class ANetworkPlayer;
class ANetworkOthers;
class UMouseCursorWidget;
class ILClickInterface;
class UCameraShakeBase;

/**
 * 
 */
UCLASS()
class MABINOGI_API ANetworkPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ANetworkPlayerController();

protected:
	/*---------------------------------------------------
								�θ� �̺�Ʈ �Լ�
	---------------------------------------------------*/

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float _DeltaTime) override;

	/*---------------------------------------------------
									�Է�
	---------------------------------------------------*/
private:
	void LeftClickCallBack();
	void RightClickStartCallBack() { IsCamRotMode = true; }
	void RightClickEndCallBack() { IsCamRotMode = false; }
	void MouseMoveCallBack(const FInputActionValue& Value);
	void CamZoomInCallBack() { CamZoom(-1.f); }
	void CamZoomOutCallBack() { CamZoom(1.f); }
	void CamZoom(float Value);
	void InputKeyBoardCallBack(PlayerInputType _Type);

private:
	class UInputMappingContext* PlayerMappingContext;
	class UInputAction* InputActions[INPUT_COUNT];
	bool IsCamRotMode = false;


	/*---------------------------------------------------
									Ÿ����
	---------------------------------------------------*/
public:
	void SetTarget(ANetworkOthers* _Target);
	ANetworkOthers* GetTarget() { return Target; }
	void ShakeCamera(float _Scale = 1.f);

private:
	ANetworkOthers* Target = nullptr;
	const float TargetArea = 3000.f;
	TSubclassOf<UCameraShakeBase> ShakeClass;

	
	/*---------------------------------------------------
									��Ʈ��ũ
	---------------------------------------------------*/
public:
	void Init(ANetworkPlayer* _Player, Protocol::CreatureType _PlayerType);

	/*---------------------------------------------------
									�̵�
	---------------------------------------------------*/
public:
	const FVector& GetDestination() { return Destination; }
	ANetworkPlayer* GetPlayer() { return Player; }
	const FHitResult& GetMouseWorldHit() { return MouseWorldHit; }

public:
	void MoveToPoint(const FVector& _Location);
	void MoveToMousePoint();

private:
	FHitResult MouseWorldHit;
	FVector Destination;
	ANetworkPlayer* Player = nullptr;


	/*---------------------------------------------------
									FSM
	---------------------------------------------------*/
public:
	void ChangeFsmState(PlayerStateType _State) { Fsm.ChangeState(_State); }
	PlayerStateType GetFsmState();

private:
	PlayerFSM Fsm;



	/*---------------------------------------------------
									���콺 Ŀ�� ����
	---------------------------------------------------*/
public:
	UMouseCursorWidget* GetCursorWidget() { return CursorWidget; }

private:
	void CheckHover(AActor* _HoverActor);

private:
	TSubclassOf<UMouseCursorWidget> MouseCursorWidgetClass;
	UMouseCursorWidget* CursorWidget;
	ILClickInterface* HoverTarget = nullptr;



};

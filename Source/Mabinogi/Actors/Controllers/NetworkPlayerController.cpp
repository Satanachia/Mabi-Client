// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controllers/NetworkPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/Network/NetworkPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "Interface/LClickInterface.h"
#include "MabiGameInstance.h"
#include "Levels/BaseLevel.h"
#include "Levels/BattleLevel.h"
#include "Actors/Network/NetworkOthers.h"
#include "UI/ObjectInfoWidget.h"
#include "UI/MouseCursorWidget.h"
#include "Levels/BattleLevel.h"
#include "UI/HUD/HUDWidget.h"
#include "UI/HUD/SkillInfoWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeBase.h"

#include "FSM/IdleState.h"
#include "FSM/MoveState.h"
#include "FSM/AttackState.h"
#include "FSM/Skill/MageSkillState.h"
#include "FSM/Skill/WarriorSkillState.h"
#include "FSM/Skill/RogSkillState.h"

ANetworkPlayerController::ANetworkPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingHelper(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Blueprints/Input/MappingContext/IMC_Player.IMC_Player'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> InputHelpers[INPUT_COUNT] =
	{
		ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/Player/IA_MovePoint.IA_MovePoint'")),
		ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/Camera/IA_CamRotMode.IA_CamRotMode'")),
		ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/Camera/IA_CamRot.IA_CamRot'")),
		ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/Camera/IA_CamZoomIn.IA_CamZoomIn'")),
		ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/Camera/IA_CamZoomOut.IA_CamZoomOut'")),
		ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/ActionSlot/IA_ActionSlot1.IA_ActionSlot1'")),
		ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/ActionSlot/IA_ActionSlot2.IA_ActionSlot2'")),
		ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/ActionSlot/IA_ActionSlot3.IA_ActionSlot3'")),
		ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/ActionSlot/IA_ActionSlot4.IA_ActionSlot4'")),
		ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/ActionSlot/IA_ActionSlot5.IA_ActionSlot5'")),
		ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/HUD/IA_SkillInfo.IA_SkillInfo'")),
		ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/HUD/IA_Inventory.IA_Inventory'")),
	};
	
	check(MappingHelper.Succeeded());
	PlayerMappingContext = MappingHelper.Object;
	
	for (int32 i = 0; i < INPUT_COUNT; ++i)
	{
		check(InputHelpers[i].Succeeded());
		InputActions[i] = InputHelpers[i].Object;
	}
	
	static ConstructorHelpers::FClassFinder<UMouseCursorWidget> MouseCursorHelper(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/WBP_MouseCursor.WBP_MouseCursor_C'"));
	check(MouseCursorHelper.Succeeded());
	MouseCursorWidgetClass = MouseCursorHelper.Class;

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> CamShakeHelper(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Controller/BP_CameraShake.BP_CameraShake_C'"));
	check(CamShakeHelper.Succeeded());
	ShakeClass = CamShakeHelper.Class;
}


/*---------------------------------------------------
								부모 이벤트 함수
---------------------------------------------------*/

void ANetworkPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(PlayerMappingContext, 0);
	
	CursorWidget = CreateWidget<UMouseCursorWidget>(this, MouseCursorWidgetClass);
	SetMouseCursorWidget(EMouseCursor::Default, CursorWidget);
}


void ANetworkPlayerController::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	//매 프레임 마우스 위치 월드좌표 지점 구하기
	if (true == GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, MouseWorldHit))
	{
		//마우스 월드좌표 디버깅 그리기
		//DrawDebugSphere(GetWorld(), MouseWorldHit.Location, 20.f, 12, FColor::Green, false, 0.f);
		CheckHover(MouseWorldHit.GetActor());
	}
	else
	{
		CheckHover(nullptr);
	}

	Fsm.Update(_DeltaTime);
}




/*---------------------------------------------------
								입력 바인딩 및 콜백
---------------------------------------------------*/

void ANetworkPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (nullptr == EnhancedInputComponent)
		return;
	
	//이동
	EnhancedInputComponent->BindAction(InputActions[INPUT_LEFT_CLICK], ETriggerEvent::Completed, this, &ANetworkPlayerController::LeftClickCallBack);
	
	//카메라 회전
	EnhancedInputComponent->BindAction(InputActions[INPUT_RIGHT_CLICK], ETriggerEvent::Started, this, &ANetworkPlayerController::RightClickStartCallBack);
	EnhancedInputComponent->BindAction(InputActions[INPUT_RIGHT_CLICK], ETriggerEvent::Completed, this, &ANetworkPlayerController::RightClickEndCallBack);
	EnhancedInputComponent->BindAction(InputActions[INPUT_MOUSE_MOVE], ETriggerEvent::Triggered, this, &ANetworkPlayerController::MouseMoveCallBack);
	
	//카메라 거리
	EnhancedInputComponent->BindAction(InputActions[INPUT_MOUSE_WHEEL_UP], ETriggerEvent::Triggered, this, &ANetworkPlayerController::CamZoomInCallBack);
	EnhancedInputComponent->BindAction(InputActions[INPUT_MOUSE_WHEEL_DOWN], ETriggerEvent::Triggered, this, &ANetworkPlayerController::CamZoomOutCallBack);

	//키보드 입력 콜백
	for (int32 i = INPUT_KEY_NUM_1; i < INPUT_COUNT; ++i)
	{
		EnhancedInputComponent->BindAction(InputActions[i], ETriggerEvent::Started, this, &ANetworkPlayerController::InputKeyBoardCallBack, static_cast<PlayerInputType>(i));
	}
}


void ANetworkPlayerController::LeftClickCallBack()
{
	SetTarget(nullptr);

	Destination = MouseWorldHit.ImpactPoint;
	AActor* HitActor = MouseWorldHit.GetActor();
	ILClickInterface* Excuter = Cast<ILClickInterface>(HitActor);
	if (nullptr != Excuter)
	{
		Excuter->ExcuteLClick(this);
	}
	
	Fsm.OnInteractionCallBack(INPUT_LEFT_CLICK);
}



void ANetworkPlayerController::MouseMoveCallBack(const FInputActionValue& Value)
{
	if (nullptr == Player)
		return;

	const float CamRotSpeed = 10.f;
	const float CamRotMinRange = -60.f;
	const float CamRotMaxRange = 10.f;

	if (false == IsCamRotMode)
		return;
	
	FVector2D MouseAxis = Value.Get<FVector2D>();
	if (true == MouseAxis.IsZero())
		return;
	
	USpringArmComponent* SpringArm = Player->GetSpringArm();
	FRotator CurCamRot = SpringArm->GetRelativeRotation();

	MouseAxis *= CamRotSpeed;
	CurCamRot.Add(MouseAxis.Y, MouseAxis.X, 0.f);
	CurCamRot.Pitch = FMath::Clamp(CurCamRot.Pitch, CamRotMinRange, CamRotMaxRange);
	
	SpringArm->SetRelativeRotation(CurCamRot);
}

void ANetworkPlayerController::CamZoom(float Value)
{
	if (nullptr == Player)
		return;

	const float CamMinArmLen = 200.f;
	const float CamMaxArmLen = 2000.f;
	const float CamArmLenSpeed = 100.f;

	USpringArmComponent* SpringArm = Player->GetSpringArm();
	float CamLen = SpringArm->TargetArmLength;
	SpringArm->TargetArmLength = FMath::Clamp(CamLen + Value * CamArmLenSpeed, CamMinArmLen, CamMaxArmLen);
}

void ANetworkPlayerController::InputKeyBoardCallBack(PlayerInputType _Type)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GetWorld()->GetGameInstance());
	ABattleLevel* Level = Cast<ABattleLevel>(GameInst->GetCurrentLevel());
	check(Level);
	UHUDWidget* HUDWidget = Level->GetHUDWidget();

	//액션 슬롯
	if(INPUT_KEY_NUM_1 <= _Type && _Type <= INPUT_KEY_NUM_5)
	{
		HUDWidget->ExcuteActionSlot(_Type - INPUT_KEY_NUM_1);
	}

	//스킬창
	else if (INPUT_KEY_K == _Type)
	{
		HUDWidget->OnOffSkillWidget();
	}

	//인벤토리
	else if (INPUT_KEY_I == _Type)
	{
		HUDWidget->OnOffInventory();
	}
	else { assert(false); }
}




/*---------------------------------------------------
							타겟팅
---------------------------------------------------*/

void ANetworkPlayerController::SetTarget(ANetworkOthers* _Target)
{
	Target = _Target;
	Fsm.ChangeState(PLAYER_STATE_MOVE);
}

void ANetworkPlayerController::ShakeCamera(float _Scale /*= 1.f*/)
{
	ClientStartCameraShake(ShakeClass, _Scale);
}

PlayerStateType ANetworkPlayerController::GetFsmState()
{
	return Fsm.GetCurState();
}


/*---------------------------------------------------
							기타
---------------------------------------------------*/


void ANetworkPlayerController::Init(ANetworkPlayer* _Player, Protocol::CreatureType _PlayerType)
{
	Player = _Player;
	check(Player);

	//FSM 추가

	// 아이들에서 이거 해줘야함
	//Protocol::C_ANIMATION AniPkt;
	//AniPkt.set_ani_type(Protocol::ANI_TYPE_MOVE);
	//SEND_PACKET(AniPkt);
	//
	
	Fsm.CreateState<IdleState>(PLAYER_STATE_IDLE, this);
	Fsm.CreateState<MoveState>(PLAYER_STATE_MOVE, this);
	Fsm.CreateState<AttackState>(PLAYER_STATE_ATTACK, this);
	Fsm.ChangeState(PLAYER_STATE_IDLE);
	
	switch (_PlayerType)
	{
	case Protocol::CREATURE_TYPE_PLAYER_MAGE:
		Fsm.CreateState<MageSkillState>(PLAYER_STATE_SKILL, this);
		break;
	case Protocol::CREATURE_TYPE_PLAYER_WARRIOR:
		Fsm.CreateState<WarriorSkillState>(PLAYER_STATE_SKILL, this);
		break;
	case Protocol::CREATURE_TYPE_PLAYER_ROG:
		Fsm.CreateState<RogSkillState>(PLAYER_STATE_SKILL, this);
		break;
	default:
	{
		assert(false);
		break;
	}
	}

	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GetWorld()->GetGameInstance());
	ABattleLevel* Level = Cast<ABattleLevel>(GameInst->GetCurrentLevel());
	check(Level);
	UHUDWidget* HUDWidget = Level->GetHUDWidget();
	USkillInfoWidget* SkillUI = HUDWidget->GetSkillUI();
	SkillUI->Init(_PlayerType);
}


void ANetworkPlayerController::MoveToMousePoint()
{
	//움직일 수 있는 상황인지 판단
	if (false == MoveState::IsMovableState(Fsm.GetCurState()))
		return;

	AActor* HitActor = MouseWorldHit.GetActor();
	if (nullptr == HitActor)
		return;

	//원하는 태그가 있는지 확인
	const TArray<FName>& HitTags = HitActor->Tags;
	if (true == HitTags.IsEmpty())
		return;
	
	if (-1 == HitTags.Find(TEXT("Walkable")))
		return;

	//목표 위치 수정
	float HalfHeight = Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector DestPos = MouseWorldHit.Location + (FVector::UpVector * HalfHeight);
	MoveToPoint(DestPos);
}



void ANetworkPlayerController::MoveToPoint(const FVector& _Location)
{
	if (false == MoveState::IsMovableState(Fsm.GetCurState()))
		return;
	
	Destination = _Location;
	Fsm.ChangeState(PLAYER_STATE_MOVE);
}


void ANetworkPlayerController::CheckHover(AActor* _HoverActor)
{
	ILClickInterface* IHover = Cast<ILClickInterface>(_HoverActor);

	//기존 타겟이 없는데 새로운 타겟이 들어온 경우
	if (nullptr != IHover && nullptr == HoverTarget)
	{
		IHover->OnHoverLMouse(this);
	}
	
	//기존 타겟이 있는데 새로운 타겟이 없는경우
	else if (nullptr == IHover && nullptr != HoverTarget)
	{
		HoverTarget->OnUnHoverLMouse(this);
	}

	//기존 타겟이 있는데 새로운 타겟이 들어온 경우
	else if (nullptr != IHover && nullptr != HoverTarget)
	{
		HoverTarget->OnUnHoverLMouse(this);
		IHover->OnHoverLMouse(this);
	}
	
	HoverTarget = IHover;
}


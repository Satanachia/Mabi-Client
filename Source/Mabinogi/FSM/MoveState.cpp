// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM/MoveState.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "Actors/Network/NetworkPlayer.h"
#include "Mabinogi.h"
#include "Components/SkeletalMeshComponent.h"
#include "AnimInst/CreatureAnimInstance.h"
#include "Actors/Network/NetworkOthers.h"
#include "Define.h"


bool MoveState::IsMovableState(PlayerStateType _State)
{
	bool Result = false;
	Result |= (PLAYER_STATE_IDLE == _State);
	Result |= (PLAYER_STATE_MOVE == _State);
	return Result;
}

void MoveState::Start()
{
	BaseState::Start();

	MoveSpeed = GetPlayer()->MoveSpeed;
}


void MoveState::Update(float _DeltaTime)
{
	BaseState::Update(_DeltaTime);

	ANetworkPlayerController* Controller = GetPlayerController();
	ANetworkPlayer* Player = Controller->GetPlayer();

	FVector StartLocation = Player->GetActorLocation(); 
	FVector DestLocation;
	ANetworkOthers* Target = Controller->GetTarget();
	if (nullptr != Target)
	{
		DestLocation = Target->GetActorLocation();
		DrawDebugSphere(Player->GetWorld(), DestLocation, AttackRangeDistance, 12, FColor::Blue, false, 0.f);
	}
	else
	{
		DestLocation = Controller->GetDestination();
	}
	DrawDebugLine(Player->GetWorld(), StartLocation, DestLocation, FColor::Green, false, 0.f);
	
	FVector MoveDir = (DestLocation - StartLocation);
	float RemainDistance = MoveDir.Length();
	float CurMoveDistance = Player->MoveSpeed * _DeltaTime;

	if (nullptr != Target && RemainDistance < AttackRangeDistance)
	{
		SendMovePacket();
		ChangeState(PLAYER_STATE_ATTACK);
		return;
	}

	//목표지점에 도착한 경우
	if (RemainDistance < CurMoveDistance)
	{
		Player->SetActorLocation(DestLocation);
		SendMovePacket();
		ChangeState(PLAYER_STATE_IDLE);
		return;
	}
	
	if (false == MoveDir.Normalize())
		return;
	
	//이동
	Player->AddMovementInput(MoveDir);

	//MovePacketSendTime초에 한번씩 이동 패킷 전송
	MovePacketTimer += _DeltaTime;
	if (MovePacketTimer < NET_CREATURE_MOVE_DURATION)
		return;
	
	//이동 패킷 전송
	MovePacketTimer = 0.f;
	SendMovePacket();
}

void MoveState::SendMovePacket()
{
	ANetworkPlayer* Player = GetPlayer();
	const FVector& CurLocation = Player->GetActorLocation();
	Protocol::PosInfo& Pos = Player->GetProtocolPos();
	Pos.set_x(CurLocation.X);
	Pos.set_y(CurLocation.Y);
	Pos.set_z(CurLocation.Z);
	
	Protocol::C_MOVE MovePkt;
	MovePkt.mutable_current_pos()->CopyFrom(Pos);
	SEND_PACKET(MovePkt);
}


void MoveState::OnInteractionCallBack(PlayerInputType _Type)
{
	BaseState::OnInteractionCallBack(_Type);

	if (PlayerInputType::INPUT_LEFT_CLICK != _Type)
		return;
	
	MovePacketTimer = 0.f;
	GetPlayerController()->MoveToMousePoint();
}



void MoveState::ExitState()
{
	BaseState::ExitState();
	MovePacketTimer = 0.f;
}


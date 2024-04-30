// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM/BaseState.h"
#include "FSM/PlayerFSM.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "Actors/Network/NetworkPlayer.h"
#include "AnimInst/PlayerAnimInstance.h"

void BaseState::Start()
{
	
}

void BaseState::EnterState()
{

}

void BaseState::Update(float _DeltaTime)
{
	
}

void BaseState::ExitState()
{

}

ANetworkPlayer* BaseState::GetPlayer()
{
	return PlayerCtrl->GetPlayer();
}

UPlayerAnimInstance* BaseState::GetPlayerAnimInst()
{
	UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(GetPlayer()->GetMesh()->GetAnimInstance());
	check(AnimInst);
	return AnimInst;
}

void BaseState::ChangeState(PlayerStateType _Key, bool _IsForce /*= false*/)
{
	int32 TempBit = OutsideChangeBit;
	OutsideChangeBit = -1;
	Fsm->ChangeState(_Key, _IsForce);
	OutsideChangeBit = TempBit;
}

PlayerStateType BaseState::GetCurrentState()
{
	return Fsm->GetCurState();
}

bool BaseState::IsThisWorking()
{
	return GetCurrentState() == Key;
}

void BaseState::BlockOutsideChange(const std::vector<PlayerStateType>& _ChangableStates /*= std::vector<PlayerStateType>()*/)
{
	OutsideChangeBit = 0;
	
	for (PlayerStateType ChangableState : _ChangableStates)
	{
		int32 StateBit = 1 << static_cast<int32>(ChangableState);
		OutsideChangeBit |= StateBit;
	}
}

bool BaseState::IsChangable(PlayerStateType _State)
{
	int32 StateBit = 1 << static_cast<int32>(_State);
	bool Result = (OutsideChangeBit & StateBit);
	return Result;
}

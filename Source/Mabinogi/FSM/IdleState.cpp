// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM/IdleState.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "Mabinogi.h"


void IdleState::OnInteractionCallBack(PlayerInputType _Type)
{
	BaseState::OnInteractionCallBack(_Type);

	if (PlayerInputType::INPUT_LEFT_CLICK != _Type)
		return;

	GetPlayerController()->MoveToMousePoint();
}

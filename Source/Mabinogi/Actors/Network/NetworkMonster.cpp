// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Network/NetworkMonster.h"
#include "Actors/Controllers/NetworkPlayerController.h"

void ANetworkMonster::ExcuteLClick(ANetworkPlayerController* _PlayerCtrl)
{
	if (false == IsAlive())
		return;
	
	_PlayerCtrl->SetTarget(this);
}

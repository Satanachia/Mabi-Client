// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Network/NetworkOthers.h"
#include "Interface/LClickInterface.h"
#include "NetworkMonster.generated.h"

class ANetworkPlayerController;

UCLASS()
class MABINOGI_API ANetworkMonster : public ANetworkOthers, public ILClickInterface
{
	GENERATED_BODY()
	
public:
	virtual void ExcuteLClick(ANetworkPlayerController* _PlayerCtrl) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Network/NetworkCreature.h"
#include "NetworkPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class MABINOGI_API ANetworkPlayer : public ANetworkCreature
{
	GENERATED_BODY()
	
public:
	ANetworkPlayer();

	virtual void InitFromType(uint32 _NetworkSecondType) override;
	
	/*--------------------
				ÄÄÆ÷³ÍÆ®
	--------------------*/

public:
	USpringArmComponent* GetSpringArm() { return SpringArm; }
	UCameraComponent* GetFollowCamera() { return FollowCamera; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
};

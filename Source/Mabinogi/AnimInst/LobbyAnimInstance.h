// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LobbyAnimInstance.generated.h"


//	UENUM(BlueprintType)
//	enum class ELobbyState : uint8
//	{
//		Idle UMETA(DisplayName = "Idle"),
//		Hover UMETA(DisplayName = "Hover"),
//		Selected UMETA(DisplayName = "Selected"),
//	};

/**
 * 
 */
UCLASS()
class MABINOGI_API ULobbyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
//	//void SetState(ELobbyState _State) { State = _State; }
//	void SetState_Idle() { State = ELobbyState::Idle; }
//	void SetState_Hover() { State = ELobbyState::Hover; }
//	void SetState_Selected() { State = ELobbyState::Selected; }
//
//private:
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta=(AllowPrivateAccess=true))
//	ELobbyState State;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Protocol.pb.h"
#include "LobbyActorBase.generated.h"


enum class LobbyActorState
{
	None,
	Idle,
	Hover,
	Selected,
};

UCLASS()
class MABINOGI_API ALobbyActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ALobbyActorBase();

	void SetAniState(LobbyActorState _State);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimationAsset* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimationAsset* HoverAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimationAsset* SelectedAnimation;
	
	const FString& GetLobbyName() { return LobbyName; }
	void SetLobbyName(const FString& _Name) { LobbyName = _Name; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	LobbyActorState CurState = LobbyActorState::None;
	FString LobbyName;
};

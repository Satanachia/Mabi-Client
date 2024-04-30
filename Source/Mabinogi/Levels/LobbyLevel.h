// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Levels/BaseLevel.h"
#include "LobbyLevel.generated.h"

class UNewPlayerCreateWidget;
class ULobbySelectPlayerWidget;

/**
 * 
 */
UCLASS()
class MABINOGI_API ALobbyLevel : public ABaseLevel
{
	GENERATED_BODY()
	
public:
	ALobbyLevel();

	void ChangeState_NewPlayer(const TFunction<void()>& _CallBack = nullptr);
	void ChangeState_SelectPlayer(const Protocol::S_LOGIN& _Pkt);
	void WaitForExitLobby();

	void CheckEnterRoomReady() { IsReadyEnterRoom = true; }

private:
	UFUNCTION()
	void CheckEnterRoom();

protected:
	TSubclassOf<ULobbySelectPlayerWidget> LobbyLogin;
	TSubclassOf<ULobbySelectPlayerWidget> SelectPlayer;
	TSubclassOf<UNewPlayerCreateWidget> NewPlayerCreate;
	
private:
	bool IsReadyEnterRoom = false;
	FTimerHandle TimeHandle;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Mabinogi.h"
#include "Managers/InventoryManager.h"
#include "MabiGameInstance.generated.h"


class ACreature;
class ABaseLevel;
class ANetworkPlayer;


/**
 * 
 */
UCLASS()
class MABINOGI_API UMabiGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	/*-------------------------------------------------
							���� ����
	-------------------------------------------------*/

	bool ConnectToGameServer(const FString& _IpAddr);
	void DisconnectToGameServer();

	//���Ź��� ��Ŷ�� ó���ϴ� �Լ�
	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	//��Ŷ �����ϱ�
	void SendPacket(SendBufferRef _SendBuffer);

	bool IsConnnected() { return (nullptr != Socket && nullptr != GameServerSession); }

	void StartPing();
	void CheckPingPong(const Protocol::S_PONG _Pkt);
	
protected:
	virtual void Shutdown() override;

public:
	TSharedPtr<class PacketSession> GameServerSession;

private://���� ����
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;
	double PrevPingTime = 0.;

public:
	/*-------------------------------------------------
							���� ����
	-------------------------------------------------*/

	void PushLevelJob(Protocol::LevelType _LevelType, const TFunction<void(ABaseLevel*)>& _Job);
	void ExcuteLevelJob(ABaseLevel* _Level, float _Duration = 0.2f);

	void SetCurrentLevel(ABaseLevel* _Level) { CurLevel = _Level; }
	ABaseLevel* GetCurrentLevel() { return CurLevel; }
	void SetMyPlayer(ANetworkPlayer* _Player) { Player = _Player; }
	ANetworkPlayer* GetMyPlayer() { return Player; }

public:
	InventoryManager InventoryMgr;
	
private:
	TQueue<TFunction<void(ABaseLevel*)>> LevelJobQueue[Protocol::LEVEL_TYPE_COUNT];
	ABaseLevel* CurLevel = nullptr;
	ANetworkPlayer* Player = nullptr;
};

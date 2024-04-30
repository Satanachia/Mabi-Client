// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Levels/BaseLevel.h"
#include "BattleLevel.generated.h"

class UBattleWidget;
class UHUDWidget;
class ANetworkOthers;

/**
 * 
 */
UCLASS()
class MABINOGI_API ABattleLevel : public ABaseLevel
{
	GENERATED_BODY()

public:
	ABattleLevel() { assert(false); }
	ABattleLevel(Protocol::LevelType _Type);

	/*-------------------------------------------------
										위젯관련
	-------------------------------------------------*/

	UHUDWidget* GetHUDWidget() { return HudWidget; }
	float GetMaxHeight() { return MaxHeight; }

private:
	UHUDWidget* HudWidget = nullptr;

	/*-------------------------------------------------
										가상함수
	-------------------------------------------------*/
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void EnterLevelArea(ANetworkPlayer* _Player, const FVector& _NextLocation);
	virtual void ExitLevelArea(ANetworkPlayer* _Player);


public:
	/*-------------------------------------------------
										서버 동기화 관련
	-------------------------------------------------*/

	//S_ENTER_GAME패킷을 바탕으로 플레이어 스폰
	void SpawnPlayer(const Protocol::S_ENTER_GAME& EnterGamePkt);

	//S_SPAWN패킷을 바탕으로 스폰
	void SpawnOther(const Protocol::S_SPAWN& SpanwPkt);

	void HandleMove(Protocol::S_MOVE& pkt);

	void HandleDespawn(uint64 ObjectId);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);

	void HandleAnimation(const Protocol::S_ANIMATION& _pkt);
	void HandleAttack(const Protocol::S_ATTACK& _AttackPkt);
	void HandleThink(uint64 _ObjId, Protocol::AttackType _attackType, bool _IsDone);
	void HandleSpawnParticle(const Protocol::S_SPAWN_PARTICLE& SpanwPkt);
	void HandleUseItem(const Protocol::S_USE_ITEM& _Pkt);

public:
	ANetworkPlayer* GetMyPlayer() { return MyPlayer; }
	ANetworkObject* FindNetworkObject(uint64 _NetId);
	void EraseNetObject(uint64 _ObjectId);

private:
	//현재 플레이어의 위치를 서버로 전송(플레이어 스폰시 위치 동기화 맞추는 용도)
	void SendPlayerLocation();

	ANetworkObject* CreateNetworkObject(uint64 _ObjectId, const FVector& _SpawnLocation);
	ANetworkOthers* CreateNetworkOther(uint64 _ObjectId, const FVector& _SpawnLocation);

private:
	ANetworkPlayer* MyPlayer = nullptr;
	float MaxHeight = 30000.f;

	//이 게임에 스폰된 플레이어들
	TMap<uint64, ANetworkObject*> NetObjects;


};

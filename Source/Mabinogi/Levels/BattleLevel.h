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
										��������
	-------------------------------------------------*/

	UHUDWidget* GetHUDWidget() { return HudWidget; }
	float GetMaxHeight() { return MaxHeight; }

private:
	UHUDWidget* HudWidget = nullptr;

	/*-------------------------------------------------
										�����Լ�
	-------------------------------------------------*/
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void EnterLevelArea(ANetworkPlayer* _Player, const FVector& _NextLocation);
	virtual void ExitLevelArea(ANetworkPlayer* _Player);


public:
	/*-------------------------------------------------
										���� ����ȭ ����
	-------------------------------------------------*/

	//S_ENTER_GAME��Ŷ�� �������� �÷��̾� ����
	void SpawnPlayer(const Protocol::S_ENTER_GAME& EnterGamePkt);

	//S_SPAWN��Ŷ�� �������� ����
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
	//���� �÷��̾��� ��ġ�� ������ ����(�÷��̾� ������ ��ġ ����ȭ ���ߴ� �뵵)
	void SendPlayerLocation();

	ANetworkObject* CreateNetworkObject(uint64 _ObjectId, const FVector& _SpawnLocation);
	ANetworkOthers* CreateNetworkOther(uint64 _ObjectId, const FVector& _SpawnLocation);

private:
	ANetworkPlayer* MyPlayer = nullptr;
	float MaxHeight = 30000.f;

	//�� ���ӿ� ������ �÷��̾��
	TMap<uint64, ANetworkObject*> NetObjects;


};

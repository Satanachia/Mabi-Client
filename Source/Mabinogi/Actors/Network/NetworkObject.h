// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Protocol.pb.h"
#include "NetworkObject.generated.h"

class ABattleLevel;

UCLASS()
class MABINOGI_API ANetworkObject : public ACharacter
{
	GENERATED_BODY()
	
public:
	/*---------------------------------------------------
								정적 함수
	---------------------------------------------------*/
	enum
	{
		FULL_BLOCK_BIT = 0xF,
		FIRST_BIT_BLOCK_POS = 60,
		SECOND_BIT_BLOCK_POS = 56
	};

	static Protocol::ObjectType FindObjectType(uint64_t _ProtocolId);
	static Protocol::CreatureType FindCreatureType(uint64_t _ProtocolId);
	static Protocol::AttackType FindProjectileType(uint64_t _ProtocolId);

	static float CheckTerrainHeight(const Protocol::PosInfo& _Pos, ABattleLevel* _Level, AActor* _Caller = nullptr, float _CallerHalfHeight = 0.f);
	
private:
	static uint32 FindSecondType(uint64_t _ProtocolId);
	
public:
	/*---------------------------------------------------
								프로토콜 아이디에 따른 초기화
	---------------------------------------------------*/
	void Init(const Protocol::ObjectInfo& _Info, bool _IsMyPlayer = false);

protected:
	virtual void InitFromType(uint32 _NetworkSecondType){}

	/*---------------------------------------------------
								Get/Set
	---------------------------------------------------*/
public:
	bool IsMyPlayer() { return IsMyPlayerValue; }
	Protocol::PosInfo& GetProtocolPos() { return ProtocolPos; }
	uint64 GetProtocolID() { return ProtocolInfo.object_id(); }
	bool IsAlive() { return 0 < ProtocolInfo.hp(); }
	const std::string& GetProtocolName() { return ProtocolInfo.object_name(); }

protected:
	Protocol::ObjectInfo& GetProtocolInfo() { return ProtocolInfo; }
	
	/*---------------------------------------------------
								컨텐츠
	---------------------------------------------------*/
public:
	virtual void LookAt(const FVector& _Pos);
	void LookAt(class AActor* _Target) { LookAt(_Target->GetActorLocation()); }
	void LookAt(const Protocol::PosInfo& _Pos) { LookAt(FVector(_Pos.x(), _Pos.y(), _Pos.z())); }

	virtual void SetMovePkt(Protocol::S_MOVE& pkt);

protected:
	float CheckTerrainHeight(const Protocol::PosInfo& _Pos);

private:
	Protocol::ObjectInfo ProtocolInfo;
	Protocol::PosInfo ProtocolPos;
	Protocol::ObjectType NetworkObjectType = Protocol::OBJECT_TYPE_NONE;
	uint32 NetworkSecondType = 0;
	bool IsMyPlayerValue = false;
};

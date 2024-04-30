// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Network/NetworkObject.h"
#include "Components/CapsuleComponent.h"
#include "Levels/BattleLevel.h"
#include "MabiGameInstance.h"
#include "Actors/StaticActors/UnmovableZone.h"

/*---------------------------------------------------
								정적 함수
---------------------------------------------------*/


Protocol::ObjectType ANetworkObject::FindObjectType(uint64_t _ProtocolId)
{
	const uint64 ObjTypeBitArea = (static_cast<uint64>(FULL_BLOCK_BIT) << FIRST_BIT_BLOCK_POS);
	const uint64 FirstAreaBit = (_ProtocolId & ObjTypeBitArea) >> FIRST_BIT_BLOCK_POS;
	
	Protocol::ObjectType ObjType = static_cast<Protocol::ObjectType>(FirstAreaBit);
	return ObjType;
}

Protocol::CreatureType ANetworkObject::FindCreatureType(uint64_t _ProtocolId)
{
	check(Protocol::OBJECT_TYPE_CREATURE == FindObjectType(_ProtocolId));
	
	uint32 SecondType = FindSecondType(_ProtocolId);
	Protocol::CreatureType CreatureType = static_cast<Protocol::CreatureType>(SecondType);
	return CreatureType;
}

Protocol::AttackType ANetworkObject::FindProjectileType(uint64_t _ProtocolId)
{
	check(Protocol::OBJECT_TYPE_PROJECTILE == FindObjectType(_ProtocolId));

	uint32 SecondType = FindSecondType(_ProtocolId);
	Protocol::AttackType AttackType = static_cast<Protocol::AttackType>(SecondType);
	return AttackType;
}


uint32 ANetworkObject::FindSecondType(uint64_t _ProtocolId)
{
	uint64 SecondBitArea = (static_cast<int64>(FULL_BLOCK_BIT) << SECOND_BIT_BLOCK_POS);
	uint64 SecondType = (_ProtocolId & SecondBitArea) >> SECOND_BIT_BLOCK_POS;
	return static_cast<int32>(SecondType);
}


float ANetworkObject::CheckTerrainHeight(const Protocol::PosInfo& _Pos, ABattleLevel* _Level, AActor* _Caller /*= nullptr*/, float _CallerHalfHeight /*= 0.f*/)
{
	const float StartHeight = _Level->GetMaxHeight();
	const float EndHeight = 0.f;

	FVector RayStart(_Pos.x(), _Pos.y(), StartHeight);
	FVector RayEnd(_Pos.x(), _Pos.y(), EndHeight);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	if (nullptr != _Caller)
	{
		CollisionParams.AddIgnoredActor(_Caller);
	}
	if (false == GWorld->LineTraceSingleByChannel(HitResult, RayStart, RayEnd, ECC_Visibility, CollisionParams))
		return FLT_MIN;

	AActor* HitActor = HitResult.GetActor();
	if (nullptr == HitActor)
		return FLT_MIN;

	FColor RayColor = FColor::Green;
	if (nullptr != Cast<AUnmovableZone>(HitActor) && nullptr != _Caller)
	{
		RayEnd.Z = _Caller->GetActorLocation().Z;
		RayColor = FColor::Red;
	}
	else if (nullptr != Cast<ANetworkObject>(HitActor))
	{
		RayEnd.Z = HitActor->GetActorLocation().Z;
		RayColor = FColor::Red;
	}
	else
	{
		RayEnd.Z = HitResult.ImpactPoint.Z;
		RayEnd.Z += _CallerHalfHeight;
	}

	//DrawDebugLine(GWorld, RayStart, RayEnd, RayColor, false, 1.f);
	return RayEnd.Z;
}

/*---------------------------------------------------
								프로토콜 아이디에 따른 초기화
---------------------------------------------------*/


void ANetworkObject::Init(const Protocol::ObjectInfo& _Info, bool _IsMyPlayer)
{
	//이미 ProtocolID가 설정되었던 경우
	uint64 InputProtocolId = _Info.object_id();
	if (0 != ProtocolInfo.object_id())
	{
		//ProtocolID는 최초 한번만 가능하다
		assert(Info.object_id() == InputProtocolId);
	}

	//프로토콜 아이디 및 이름 등의 정보 설정
	ProtocolInfo.CopyFrom(_Info);
	FVector SpawnLocation = GetActorLocation();
	ProtocolPos.set_x(SpawnLocation.X);
	ProtocolPos.set_y(SpawnLocation.Y);
	ProtocolPos.set_z(SpawnLocation.Z);

	//오브젝트 타입 및 하위 타입 설정
	NetworkObjectType = FindObjectType(InputProtocolId);
	NetworkSecondType = FindSecondType(InputProtocolId);
	
	//타입에 따른 처리
	InitFromType(NetworkSecondType);
	
	IsMyPlayerValue = _IsMyPlayer;
}



/*---------------------------------------------------
								컨텐츠
---------------------------------------------------*/

void ANetworkObject::LookAt(const FVector& _Pos)
{
	FVector PlayerLocation = GetActorLocation();
	FVector DirVec = (_Pos - PlayerLocation);

	DirVec.Z = 0.f;
	DirVec.Normalize();
	FRotator Rot = DirVec.Rotation();
	SetActorRotation(Rot);
}

void ANetworkObject::SetMovePkt(Protocol::S_MOVE& pkt)
{
	Protocol::ObjectInfo& NowInfo = GetProtocolInfo();
	Protocol::PosInfo& PosInfo = GetProtocolPos();
	
	//동일한 아이디인지 확인
	check(NowInfo.object_id() == pkt.object_id());

	//높이 값이 0인 경우
	Protocol::PosInfo DestPos = pkt.current_pos();
	if (0.f == DestPos.z())
	{
		float Z = CheckTerrainHeight(DestPos);
		DestPos.set_z(Z);
	}

	//위치 수정
	PosInfo.CopyFrom(DestPos);
}

float ANetworkObject::CheckTerrainHeight(const Protocol::PosInfo& _Pos)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GetWorld()->GetGameInstance());
	ABattleLevel* Level = Cast<ABattleLevel>(GameInst->GetCurrentLevel());
	check(Level);
	
	float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	return CheckTerrainHeight(_Pos, Level, this, CapsuleHalfHeight);
}

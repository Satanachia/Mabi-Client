// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/BattleLevel.h"
#include "UI/BattleWidget.h"
#include "UI/HUD/HUDWidget.h"
#include "Actors/StaticActors/ParticleActor.h"
#include "MabiGameInstance.h"
#include "UI/HUD/HUDWidget.h"
#include "UI/HUD/InventoryWidget.h"
#include "Mabinogi.h"
#include "Actors/Network/NetworkPlayer.h"
#include "Actors/Network/NetworkOthers.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "AnimInst/CreatureAnimInstance.h"
#include "UI/ObjectInfoWidget.h"
#include "Actors/Network/NetworkProjectile.h"
#include "Actors/Network/NetworkItem.h"
#include "Actors/Network/NetworkMonster.h"
#include "Managers/ResourceManager.h"

ABattleLevel::ABattleLevel(Protocol::LevelType _Type)
	:ABaseLevel(_Type)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> BattleWidgetHelper(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/HUD/WBP_HUD.WBP_HUD_C'"));
	check(BattleWidgetHelper.Succeeded());
	SetStartWidgetClass(BattleWidgetHelper.Class);
}

/*-------------------------------------------------
										위젯관련
-------------------------------------------------*/


void ABattleLevel::BeginPlay()
{
	Super::BeginPlay();

	HudWidget = Cast<UHUDWidget>(GetCurrentWidget());
	check(HudWidget);
}

/*-------------------------------------------------
										가상함수
-------------------------------------------------*/

void ABattleLevel::EnterLevelArea(ANetworkPlayer* _Player, const FVector& _NextLocation)
{
	check(Protocol::LEVEL_TYPE_NONE != GetLevelType());
	MyPlayer = _Player;
	NetObjects.Add(MyPlayer->GetProtocolID(), _Player);

	//게임 인스턴스에 자신을 현재레벨로 등록
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GetWorld()->GetGameInstance());
	check(GameInst);
	GameInst->SetCurrentLevel(this);


	//플레이어 위치 설정 및 상태 변경
	float HalfHeight = _Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	_Player->SetActorLocation(_NextLocation + (FVector::UpVector * HalfHeight));
	ANetworkPlayerController* PlayerCtrl = Cast<ANetworkPlayerController>(_Player->GetController());
	check(PlayerCtrl);
	PlayerCtrl->ChangeFsmState(PLAYER_STATE_IDLE);


	//레벨전환 패킷 전송
	Protocol::C_CHANGE_LEVEL ChangeLevelPkt;
	ChangeLevelPkt.set_next_level(GetLevelType());
	Protocol::PosInfo* SpawnPos = ChangeLevelPkt.mutable_spawn_pos();
	SpawnPos->set_x(_NextLocation.X);
	SpawnPos->set_y(_NextLocation.Y);
	SpawnPos->set_z(_NextLocation.Z + HalfHeight);
	SEND_PACKET(ChangeLevelPkt);

	SendPlayerLocation();
}

void ABattleLevel::ExitLevelArea(ANetworkPlayer* _Player)
{
	for (const TPair<uint64, ANetworkObject*>& Pair : NetObjects)
	{
		ANetworkObject* Actor = Pair.Value;
		if (_Player == Actor)
			continue;

		Actor->Destroy();
	}

	NetObjects.Empty();
}

/*-------------------------------------------------
										서버 동기화 관련
-------------------------------------------------*/

void ABattleLevel::SpawnPlayer(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	assert(GameInst);
	if (false == GameInst->IsConnnected())
		return;

	const Protocol::ObjectInfo& ObjectInfo = EnterGamePkt.player();

	//중복 처리 체크
	const uint64 ObjectId = ObjectInfo.object_id();
	if (nullptr != FindNetworkObject(ObjectId))
		return;

	//현재 활성화되어 있는 컨트롤러를 가져온다
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ANetworkPlayer* Player = Cast<ANetworkPlayer>(PlayerController->GetPawn());
	check(Player);

	//참고로 게임모드를 통해 내 플레이어는 게임에 미리 생성되어 있음
	Player->Init(ObjectInfo, true);
	MyPlayer = Player;

	//위치 맞추기(Move패킷을 한번만 보내기)
	SendPlayerLocation();
	NetObjects.Add(ObjectId, Player);
}

void ABattleLevel::SpawnOther(const Protocol::S_SPAWN& SpanwPkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	assert(GameInst);
	if (false == GameInst->IsConnnected())
		return;

	check(SpanwPkt.players_size() == SpanwPkt.positions_size());
	for (int32 i = 0; i < SpanwPkt.players_size(); ++i)
	{
		const Protocol::ObjectInfo& ObjectInfo = SpanwPkt.players(i);
		const Protocol::PosInfo& PosInfo = SpanwPkt.positions(i);

		//중복 처리 체크
		const uint64 ObjectId = ObjectInfo.object_id();
		if (nullptr != FindNetworkObject(ObjectId))
			continue;

		FVector SpawnLocation = FVector(PosInfo.x(), PosInfo.y(), PosInfo.z());
		ANetworkObject* Other = CreateNetworkObject(ObjectId, SpawnLocation);
		check(Other);

		Other->Init(ObjectInfo, false);
		NetObjects.Add(ObjectId, Other);
	}
}


ANetworkObject* ABattleLevel::CreateNetworkObject(uint64 _ObjectId, const FVector& _SpawnLocation)
{
	ANetworkObject* Result = nullptr;
	Protocol::ObjectType ObjType = ANetworkObject::FindObjectType(_ObjectId);
	switch (ObjType)
	{
	case Protocol::OBJECT_TYPE_CREATURE:
		Result = CreateNetworkOther(_ObjectId, _SpawnLocation);
		break;
	case Protocol::OBJECT_TYPE_PROJECTILE:
		Result = Cast<ANetworkObject>(GetWorld()->SpawnActor(ANetworkProjectile::StaticClass()));
		break;
	case Protocol::OBJECT_TYPE_ITEM:
		Result = Cast<ANetworkItem>(GetWorld()->SpawnActor(ANetworkItem::StaticClass()));
		break;
	}

	check(Result);
	Result->SetActorLocation(_SpawnLocation);
	return Result;
}

ANetworkOthers* ABattleLevel::CreateNetworkOther(uint64 _ObjectId, const FVector& _SpawnLocation)
{
	ANetworkOthers* Result = nullptr;
	Protocol::CreatureType CreatureType = ANetworkObject::FindCreatureType(_ObjectId);
	switch (CreatureType)
	{
	case Protocol::CREATURE_TYPE_PLAYER_MAGE:
	case Protocol::CREATURE_TYPE_PLAYER_WARRIOR:
	case Protocol::CREATURE_TYPE_PLAYER_ROG:
		Result = Cast<ANetworkOthers>(GetWorld()->SpawnActor(ANetworkOthers::StaticClass()));
		break;
	case Protocol::CREATURE_TYPE_MONSTER_IGGY:
	case Protocol::CREATURE_TYPE_MONSTER_BOSS_NAME_UNKNOWN:
		Result = Cast<ANetworkOthers>(GetWorld()->SpawnActor(ANetworkMonster::StaticClass()));
		break;
	}

	check(Result);
	Result->SetActorLocation(_SpawnLocation);
	return Result;
}

void ABattleLevel::HandleMove(Protocol::S_MOVE& pkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	assert(GameInst);
	if (false == GameInst->IsConnnected())
		return;

	UWorld* World = GetWorld();
	if (nullptr == World)
		return;

	const uint64 ObjectId = pkt.object_id();
	ANetworkObject* Creature = FindNetworkObject(ObjectId);
	if (nullptr == Creature)
		return;

	//내가 조종하는 플레이어면 더이상 실행하지 않음
	if (true == Creature->IsMyPlayer())
		return;

	Creature->SetMovePkt(pkt);
}

void ABattleLevel::HandleDespawn(uint64 ObjectId)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	assert(GameInst);
	if (false == GameInst->IsConnnected())
		return;

	ANetworkObject* FindActor = FindNetworkObject(ObjectId);
	if (nullptr == FindActor)
		return;

	if (MyPlayer == FindActor)
		return;

	NetObjects.Remove(ObjectId);
	GetWorld()->DestroyActor(FindActor);
}

void ABattleLevel::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	for (const uint64& ObjectId : DespawnPkt.object_ids())
	{
		HandleDespawn(ObjectId);
	}
}

void ABattleLevel::HandleAnimation(const Protocol::S_ANIMATION& _pkt)
{
	ANetworkCreature* NetCreature = Cast<ANetworkCreature>(FindNetworkObject(_pkt.object_id()));
	if (nullptr == NetCreature)
		return;
	
	UCreatureAnimInstance* AnimInst = Cast<UCreatureAnimInstance>(NetCreature->GetMesh()->GetAnimInstance());
	check(AnimInst);
	Protocol::AnimationType AniType = _pkt.ani_type();
	AnimInst->HandleAnimationPacket(AniType);
}

void ABattleLevel::HandleAttack(const Protocol::S_ATTACK& _AttackPkt)
{
	ANetworkCreature* NetCreature = Cast<ANetworkCreature>(FindNetworkObject(_AttackPkt.victim_id()));
	if (nullptr == NetCreature)
		return;
	
	int32 Hp = _AttackPkt.hp();
	NetCreature->SetHp(Hp);
	CreatureParticleType Particle = 0 == Hp ? CreatureParticleType::Death : CreatureParticleType::HitReact;
	NetCreature->PlayParticle(Particle);
}


void ABattleLevel::HandleThink(uint64 _ObjId, Protocol::AttackType _attackType, bool _IsDone)
{
	ANetworkObject* FindObj = FindNetworkObject(_ObjId);
	if (nullptr == FindObj)
		return;

	ANetworkCreature* Creature = Cast<ANetworkCreature>(FindObj);
	check(Creature);

	UObjectInfoWidget* InfoUI = Creature->GetInfoUI();
	if (false == _IsDone)
	{
		InfoUI->StartThink(_attackType);
	}
	else
	{
		InfoUI->FinishThink(_attackType);
	}
	
	if (false == Creature->IsMyPlayer())
		return;

	if (false == _IsDone)
	{
		//ResourceManager::PlaySound(SoundType::ThinkReady, GetWorld());
	}
	else
	{
		ANetworkPlayerController* PlayerCtrl = Cast<ANetworkPlayerController>(GetWorld()->GetFirstPlayerController());
		check(PlayerCtrl);
		PlayerCtrl->ChangeFsmState(PLAYER_STATE_SKILL);

		ResourceManager::PlaySound(SoundType::ThinkDone, GetWorld());
	}
}

void ABattleLevel::SendPlayerLocation()
{
	check(MyPlayer);

	//현재 위치 설정
	const FVector& CurLocation = MyPlayer->GetActorLocation();
	Protocol::PosInfo& CurPos = MyPlayer->GetProtocolPos();
	CurPos.set_x(CurLocation.X);
	CurPos.set_y(CurLocation.Y);
	CurPos.set_z(CurLocation.Z);

	Protocol::C_MOVE MovePkt;
	Protocol::PosInfo* MovePosInfo = MovePkt.mutable_current_pos();
	MovePosInfo->CopyFrom(CurPos);
	SEND_PACKET(MovePkt);
}



ANetworkObject* ABattleLevel::FindNetworkObject(uint64 _NetId)
{
	ANetworkObject** ObjIter = NetObjects.Find(_NetId);
	if (nullptr == ObjIter)
		return nullptr;

	return *ObjIter;
}

void ABattleLevel::HandleSpawnParticle(const Protocol::S_SPAWN_PARTICLE& SpanwPkt)
{
	AParticleActor* Particle = Cast<AParticleActor>(GetWorld()->SpawnActor(AParticleActor::StaticClass()));
	Particle->Init(SpanwPkt);
}


void ABattleLevel::HandleUseItem(const Protocol::S_USE_ITEM& _Pkt)
{
	ANetworkObject* NetObj = FindNetworkObject(_Pkt.object_id());
	ANetworkCreature* NetCreature = Cast<ANetworkCreature>(NetObj);
	if (nullptr == NetCreature)
		return;
	
	NetCreature->GetInfoUI()->SetHp(_Pkt.hp());
}


void ABattleLevel::EraseNetObject(uint64 _ObjectId)
{
	if (false == NetObjects.Contains(_ObjectId))
		return;

	NetObjects.Remove(_ObjectId);
}

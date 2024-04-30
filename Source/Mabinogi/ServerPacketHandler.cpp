#include "ServerPacketHandler.h"
#include "Mabinogi.h"
#include "UI/LobbyWidget.h"
#include "UI/FadeWidget.h"
#include "Levels/BaseLevel.h"
#include "GameFramework/GameModeBase.h"
#include "Levels/LobbyLevel.h"
#include "Levels/BattleLevel.h"
#include "Actors/Network/NetworkCreature.h"
#include "PacketSession.h"



PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	//TODO : Log
	return false;
}


/*---------------------------------------------------------------------------------
												로비/계정
---------------------------------------------------------------------------------*/

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt) 
{
	/*
		S_LOGIN패킷의 object_id에는 실제 오브젝트 아이디가 존재하지 않고
		접속한 유저의 캐릭터들의 직업이 들어있음
	*/

	//월드부터 타고타고 가서 현재 위젯에 접근
	ABaseLevel* Level = Cast<UMabiGameInstance>(GWorld->GetGameInstance())->GetCurrentLevel();
	ALobbyLevel* LobbyLevel = Cast<ALobbyLevel>(Level);
	check(LobbyLevel);
	UUserWidget* CurWidget = LobbyLevel->GetCurrentWidget();
	assert(CurWidget);

	//로그인 실패한 경우
	if (false == pkt.success())
	{
		ULobbyWidget* LobbyWidget = Cast<ULobbyWidget>(CurWidget);
		assert(LobbyWidget);
		LobbyWidget->FailedLogin();
		return true;
	}
	
	//로그인은 성공했으나 만들어둔 플레이어가 없는 경우
	if (0 == pkt.players_size())
		LobbyLevel->ChangeState_NewPlayer();

	//캐릭터 선택창으로 이동
	else
		LobbyLevel->ChangeState_SelectPlayer(pkt);

	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	check(pkt.success());

	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	ABaseLevel* Level = GameInst->GetCurrentLevel();
	ALobbyLevel* LobbyLevel = Cast<ALobbyLevel>(Level);
	check(LobbyLevel);
	LobbyLevel->CheckEnterRoomReady();
	
	//레벨 전환 대기 시간이 있어서 레벨마다 존재하는 큐에다가 패킷 처리에 대한 행동을 임시 저장
	GameInst->PushLevelJob(Protocol::LEVEL_TYPE_START_BRIDGE, [pkt](ABaseLevel* _Level)
	{
		ABattleLevel* Level = Cast<ABattleLevel>(_Level);
		check(Level);
		Level->SpawnPlayer(pkt);
	});

	GameInst->StartPing();
	return true;
}

bool Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	check(GameInst);
	GameInst->DisconnectToGameServer();
	return false;
}


/*---------------------------------------------------------------------------------
												월드
---------------------------------------------------------------------------------*/



bool Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	GameInst->PushLevelJob(pkt.level(), [pkt](ABaseLevel* _Level)
	{
		ABattleLevel* Level = Cast<ABattleLevel>(_Level);
		check(Level);
		Level->SpawnOther(pkt);
	});

	return false;
}

bool Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	ABattleLevel* Level = Cast<ABattleLevel>(GameInst->GetCurrentLevel());
	if (nullptr == Level)
		return false;
	
	Level->HandleDespawn(pkt);
	return true;
}

bool Handle_S_MOVE(PacketSessionRef& session, Protocol::S_MOVE& pkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	ABattleLevel* Level = Cast<ABattleLevel>(GameInst->GetCurrentLevel());
	if (nullptr == Level)
		return false;

	Level->HandleMove(pkt);
	return true;
}

bool Handle_S_LOOK(PacketSessionRef& session, Protocol::S_LOOK& pkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	ABattleLevel* Level = Cast<ABattleLevel>(GameInst->GetCurrentLevel());
	if (nullptr == Level)
		return false;

	ANetworkCreature* FindObj = Cast<ANetworkCreature>(Level->FindNetworkObject(pkt.object_id()));
	if (nullptr == FindObj)
		return false;

	if (true == pkt.has_look_pos())
	{
		FindObj->LookAt(pkt.look_pos());
	}
	else if (ANetworkObject* LookTarget = Level->FindNetworkObject(pkt.look_target()))
	{
		FindObj->LookAt(LookTarget->GetActorLocation());
	}
	else
	{
		FindObj->SetActorRotation(FRotator(0.f, pkt.yaw(), 0.f));
	}
	
	return true;
}

bool Handle_S_ANIMATION(PacketSessionRef& session, Protocol::S_ANIMATION& pkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	ABattleLevel* Level = Cast<ABattleLevel>(GameInst->GetCurrentLevel());
	if (nullptr == Level)
		return false;

	Level->HandleAnimation(pkt);
	return true;
}




/*---------------------------------------------------------------------------------
												전투
---------------------------------------------------------------------------------*/

bool Handle_S_ATTACK(PacketSessionRef& session, Protocol::S_ATTACK& pkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	ABattleLevel* Level = Cast<ABattleLevel>(GameInst->GetCurrentLevel());
	if (nullptr == Level)
		return false;
	
	Level->HandleAttack(pkt);
	return true;
}

bool Handle_S_THINK(PacketSessionRef& session, Protocol::S_THINK& pkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	ABattleLevel* Level = Cast<ABattleLevel>(GameInst->GetCurrentLevel());
	if (nullptr == Level)
		return false;

	Level->HandleThink(pkt.object_id(), pkt.attack_type(), false);
	return true;
}

bool Handle_S_THINK_DONE(PacketSessionRef& session, Protocol::S_THINK_DONE& pkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	ABattleLevel* Level = Cast<ABattleLevel>(GameInst->GetCurrentLevel());
	if (nullptr == Level)
		return false;

	Level->HandleThink(pkt.object_id(), pkt.attack_type(), true);
	return true;
}

bool Handle_S_SPAWN_PARTICLE(PacketSessionRef& session, Protocol::S_SPAWN_PARTICLE& pkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	ABattleLevel* Level = Cast<ABattleLevel>(GameInst->GetCurrentLevel());
	if (nullptr == Level)
		return false;

	if (Level->GetLevelType() != pkt.level())
		return false;
	
	Level->HandleSpawnParticle(pkt);
	return true;
}


/*---------------------------------------------------------------------------------
												인벤토리
---------------------------------------------------------------------------------*/

bool Handle_S_INIT_INVENTORY(PacketSessionRef& session, Protocol::S_INIT_INVENTORY& pkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	check(GameInst);
	GameInst->InventoryMgr.Handle_Init_Inventory(pkt);
	return true;
}


bool Handle_S_ADD_ITEM(PacketSessionRef& session, Protocol::S_ADD_ITEM& pkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	check(GameInst);
	InventoryManager& InvenMgr = GameInst->InventoryMgr;
	for (int32 i = 0; i < pkt.items_size(); ++i)
	{
		const Protocol::ItemInfo& Item = pkt.items(i);
		InvenMgr.AcquireItem(Item);
	}

	return true;
}

bool Handle_S_USE_ITEM(PacketSessionRef& session, Protocol::S_USE_ITEM& pkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	ABattleLevel* Level = Cast<ABattleLevel>(GameInst->GetCurrentLevel());
	if (nullptr == Level)
		return false;
	
	Level->HandleUseItem(pkt);
	return true;
}

/*---------------------------------------------------------------------------------
												
---------------------------------------------------------------------------------*/

bool Handle_S_PONG(PacketSessionRef& session, Protocol::S_PONG& pkt)
{
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
	check(GameInst);
	//GameInst->CheckPingPong(pkt);
	return true;
}


bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	const std::string& Msg = pkt.msg();
	UE_LOG(LogTemp, Log, TEXT("Receive Chat. [Size : %d]"), Msg.size());
	
	return true;
}

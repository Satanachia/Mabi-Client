// Fill out your copyright notice in the Description page of Project Settings.


#include "MabiGameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "ServerPacketHandler.h"
#include "GameMode/MabiGameModeBase.h"
#include "Levels/BaseLevel.h"





bool UMabiGameInstance::ConnectToGameServer(const FString& _IpAddr)
{
	//이미 연결된 경우
	if (nullptr != Socket && nullptr != GameServerSession)
		return true;

	if (false == _IpAddr.IsEmpty())
		IpAddress = _IpAddr;

	//소켓생성
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	//연결할 네트워크 주소 설정
	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	//소켓에 주소/포트 연동
	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	//연결시도(Connect함수는 블로킹 함수)
	bool Connected = Socket->Connect(*InternetAddr);
	if (true == Connected)
	{
		// Session만들기
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();
	}
	else
	{
		//연결 실패 메세지 출력
		SCREEN_ERROR_MSG("Connection Failed");
	}

	return Connected;
}

void UMabiGameInstance::DisconnectToGameServer()
{
	if (false == IsConnnected())
		return;

	GameServerSession->Disconnect();
	if (nullptr != Socket)
	{
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
	}
}


void UMabiGameInstance::HandleRecvPackets()
{
	if (nullptr == Socket || nullptr == GameServerSession)
		return;

	GameServerSession->HandleRecvPackets();
}

void UMabiGameInstance::SendPacket(SendBufferRef _SendBuffer)
{
	if (nullptr == Socket || nullptr == GameServerSession)
		return;

	GameServerSession->SendPacket(_SendBuffer);
}



void UMabiGameInstance::PushLevelJob(Protocol::LevelType _LevelType, const TFunction<void(ABaseLevel*)>& _Job)
{
	check(Protocol::LEVEL_TYPE_NONE != _LevelType);
	check(Protocol::LEVEL_TYPE_COUNT != _LevelType);
	check(_Job);
	
	LevelJobQueue[_LevelType].Enqueue(_Job);
}

void UMabiGameInstance::ExcuteLevelJob(ABaseLevel* _Level, float _Duration /*= 0.2f*/)
{
	Protocol::LevelType LevelType = _Level->GetLevelType();
	check(Protocol::LEVEL_TYPE_COUNT != LevelType);

	double EndTime = FPlatformTime::Seconds() + _Duration;
	TQueue<TFunction<void(ABaseLevel*)>>& JobQueue = LevelJobQueue[LevelType];
	while (false == JobQueue.IsEmpty() && FPlatformTime::Seconds() < EndTime)
	{
		TFunction<void(ABaseLevel*)> OutCallback = nullptr;
		JobQueue.Dequeue(OUT OutCallback);
		check(OutCallback);

		OutCallback(_Level);
	}
}

void UMabiGameInstance::StartPing()
{
	PrevPingTime = GetWorld()->GetRealTimeSeconds();
	Protocol::C_PING Pkt;
	SEND_PACKET(Pkt);
}

void UMabiGameInstance::CheckPingPong(const Protocol::S_PONG _Pkt)
{
	double NowPingTime = GetWorld()->GetRealTimeSeconds();
	int32 Ping = static_cast<int32>((NowPingTime - PrevPingTime) * 1000.0);

	const float DelaySeconds = 0.2f; 
	GEngine->AddOnScreenDebugMessage(-1, DelaySeconds, FColor::Green, FString::Printf(TEXT("Ping : %d"), Ping));

	// 타이머 설정
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, []()
	{
		UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(GWorld->GetGameInstance());
		if (nullptr == GameInst)
			return;

		GameInst->StartPing();
	}, DelaySeconds, false);
}

void UMabiGameInstance::Shutdown()
{
	Super::Shutdown();

	if (nullptr != Player)
	{
		InventoryMgr.SaveInventory(Player);
	}
	
	if (false == IsConnnected())
		return;

	Protocol::C_LEAVE_GAME LeavePkt;
	SEND_PACKET(LeavePkt);
}
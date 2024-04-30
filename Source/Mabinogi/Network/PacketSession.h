// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mabinogi.h"

/**
	
	TSharedFromThis는 언리얼에서 만든 std::enable_shared_from_this
 */
class MABINOGI_API PacketSession : public TSharedFromThis<PacketSession>
{
public:
	PacketSession(class FSocket* _Socket);
	~PacketSession();

	//FSocket은 블로킹 방식의 소켓이다. 그래서 별도의 스레드를 만들어 서버와 송수신을 처리함
	void Run();

	//패킷 처리
	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	//패킷 전송
	void SendPacket(SendBufferRef _SendBuffer);

	void Disconnect();

public:
	class FSocket* Socket;

	TSharedPtr<class RecvWorker> RecvWorkerThread;
	TSharedPtr<class SendWorker> SendWorkerThread;

	//NetworkWorker쪽 스레드로부터 수신받은 데이터를 메인스레드에서 사용하기 위한 큐(내부에서 모드에 따라 락프리(락프리가 기본상태))
	TQueue<TArray<uint8>> RecvPacketQueue;
	//전송할 패킷들을 담아놓는 큐
	TQueue<SendBufferRef> SendPacketQueue;
};

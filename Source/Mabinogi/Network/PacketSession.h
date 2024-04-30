// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mabinogi.h"

/**
	
	TSharedFromThis�� �𸮾󿡼� ���� std::enable_shared_from_this
 */
class MABINOGI_API PacketSession : public TSharedFromThis<PacketSession>
{
public:
	PacketSession(class FSocket* _Socket);
	~PacketSession();

	//FSocket�� ���ŷ ����� �����̴�. �׷��� ������ �����带 ����� ������ �ۼ����� ó����
	void Run();

	//��Ŷ ó��
	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	//��Ŷ ����
	void SendPacket(SendBufferRef _SendBuffer);

	void Disconnect();

public:
	class FSocket* Socket;

	TSharedPtr<class RecvWorker> RecvWorkerThread;
	TSharedPtr<class SendWorker> SendWorkerThread;

	//NetworkWorker�� ������κ��� ���Ź��� �����͸� ���ν����忡�� ����ϱ� ���� ť(���ο��� ��忡 ���� ������(�������� �⺻����))
	TQueue<TArray<uint8>> RecvPacketQueue;
	//������ ��Ŷ���� ��Ƴ��� ť
	TQueue<SendBufferRef> SendPacketQueue;
};

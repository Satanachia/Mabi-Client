// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/PacketSession.h"
#include "NetworkWorker.h"
#include "ServerPacketHandler.h"

PacketSession::PacketSession(class FSocket* _Socket)
	:Socket(_Socket)
{
	ServerPacketHandler::Init();
}

PacketSession::~PacketSession()
{
	Disconnect();
}

void PacketSession::Run()
{
	//������ ����(���ο��� override�� Init, Run, Exit�� ȣ���Ŵ)
	RecvWorkerThread = MakeShared<RecvWorker>(Socket, AsShared());
	SendWorkerThread = MakeShared<SendWorker>(Socket, AsShared());
}

void PacketSession::HandleRecvPackets()
{
	while (true)
	{
		TArray<uint8> Packet;
		if (false == RecvPacketQueue.Dequeue(OUT Packet))
			break;
		
		//��Ŷ ó��
		PacketSessionRef ThisPtr = AsShared();
		ServerPacketHandler::HandlePacket(ThisPtr, Packet.GetData(), Packet.Num());
	}
}

void PacketSession::SendPacket(SendBufferRef _SendBuffer)
{
	SendPacketQueue.Enqueue(_SendBuffer);
}

void PacketSession::Disconnect()
{
	if (nullptr != RecvWorkerThread)
	{
		RecvWorkerThread->Destroy();
		RecvWorkerThread = nullptr;
	}

	if (nullptr != SendWorkerThread)
	{
		SendWorkerThread->Destroy();
		SendWorkerThread = nullptr;
	}
}

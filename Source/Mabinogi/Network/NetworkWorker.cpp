// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/NetworkWorker.h"
#include "Sockets.h"
#include "Serialization/ArrayWriter.h"
#include "PacketSession.h"
#include "Define.h"

/*---------------------------------------------
							RecvWorker
---------------------------------------------*/

RecvWorker::RecvWorker(FSocket* _Socket, TSharedPtr<PacketSession> _Session)
	:Socket(_Socket)
	, SessionRef(_Session)
{
	Thread = FRunnableThread::Create(this, TEXT("RecvWorkerThread"));
}

RecvWorker::~RecvWorker()
{
}

bool RecvWorker::Init()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Thread Init")));
	return true;
}

uint32 RecvWorker::Run()
{
	while (true == bRunning)
	{
		TArray<uint8> Packet;
		if (false == ReceivePacket(OUT Packet))
			continue;

		TSharedPtr<PacketSession> Session = SessionRef.Pin();
		if (nullptr == Session)
			continue;
		
		Session->RecvPacketQueue.Enqueue(Packet);
	}

	return 0;
}

void RecvWorker::Exit()
{

}

void RecvWorker::Destroy()
{
	bRunning = false;
}

bool RecvWorker::ReceivePacket(TArray<uint8>& _OutPacket)
{
	//HeaderSize��ŭ HeaderBuffer�� Resize
	const int32 HeaderSize = sizeof(FPacketHeader);
	TArray<uint8> HeaderBuffer;
	HeaderBuffer.AddZeroed(HeaderSize);

	//��Ŷ ��� �Ľ�(4Byte)
	if (false == ReceiveDesiredBytes(HeaderBuffer.GetData(), HeaderSize))
		return false;

	//ID �� Size����
	FPacketHeader Header;
	{
		//HeaderBuffer�� �ִ� �����͸�  Header�� ��������
		FMemoryReader Reader(HeaderBuffer);
		Reader << Header;
		//MSG_LOG_TEMP(TEXT("Recv PacketID : %d, PacketSize : %d"), Header.PacketID, Header.PacketSize);
	}

	//ID �� Size����
	_OutPacket = HeaderBuffer;

	//��Ŷ ���� �Ľ�
	TArray<uint8> PayloadBuffer;
	const int32 PayLoadSize = Header.PacketSize - HeaderSize;
	
	//��Ŷ ���̵� ������ ó���ϰ�,
	// ��Ŷ ��� �̿ܿ� ���� ������ ���� ��Ŷ�� ��쿣 �ٷ� ������
	if (0 == PayLoadSize)
		return true;

	_OutPacket.AddZeroed(PayLoadSize);	//�߰� ���� �Ҵ�

	if (true == ReceiveDesiredBytes(&_OutPacket[HeaderSize], PayLoadSize))
		return true;

	return false;
}

bool RecvWorker::ReceiveDesiredBytes(uint8* _Result, int32 _Size)
{
	//���Ź��� �����Ͱ� ���ų�, ������ ����Ǿ��ٸ� return false
	uint32 PendingDataSize;
	if (false == Socket->HasPendingData(PendingDataSize) || PendingDataSize <= 0)
		return false;

	uint32 Offset = 0;
	while (0 < _Size)
	{
		//NumRead : ��ŭ ���Ź޾Ҵ���
		int32 NumRead = 0;

		//���Ź��� �����͸� _Result + Offset�� _Sizeũ�⸸ŭ ����
		//TCP��� _Sizeũ�⸸ŭ �����Ͱ� �� ���� �� ����
		//�� ����ó���� �Ʒ����� Ȯ��
		Socket->Recv(_Result + Offset, _Size, OUT NumRead);

		//�о���� ���� ������ ������ ū ���� ���� �ȵ�(����ó��)
		check(NumRead <= _Size);

		//����Ǵ� ���(����ó��)
		if (NumRead <= 0)
			return false;

		//���� while���� ���� Offset�� _Size ��ġ/ũ�� ����
		Offset += NumRead;
		_Size -= NumRead;
	}

	return true;
}


/*---------------------------------------------
							SendWorker
---------------------------------------------*/

SendWorker::SendWorker(FSocket* _Socket, TSharedPtr<class PacketSession> _Session)
	:Socket(_Socket)
	, SessionRef(_Session)
{
	Thread = FRunnableThread::Create(this, TEXT("SendWorkerThread"));
}

SendWorker::~SendWorker()
{
}

bool SendWorker::Init()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Send Thread Init")));
	return true;
}

uint32 SendWorker::Run()
{
	while (true == bRunning)
	{
		SendBufferRef SendBuffer;
		TSharedPtr<PacketSession> Session = SessionRef.Pin();
		if (nullptr == Session)
			continue;

		if (false == Session->SendPacketQueue.Dequeue(OUT SendBuffer))
			continue;

		SendDesiredBytes(SendBuffer->Buffer(), SendBuffer->WriteSize());
	}

	return 0;
}

void SendWorker::Exit()
{
}

void SendWorker::Destroy()
{
	bRunning = false;
}

bool SendWorker::SendDesiredBytes(const uint8* _Buffer, int32 _Size)
{
	while (0 < _Size)
	{
		int32 BytesSent = 0;
		if (false == Socket->Send(_Buffer, _Size, BytesSent))
			return false;

		_Size -= BytesSent;
		_Buffer += BytesSent;
	}

	return true;
}

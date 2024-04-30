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
	//HeaderSize만큼 HeaderBuffer를 Resize
	const int32 HeaderSize = sizeof(FPacketHeader);
	TArray<uint8> HeaderBuffer;
	HeaderBuffer.AddZeroed(HeaderSize);

	//패킷 헤더 파싱(4Byte)
	if (false == ReceiveDesiredBytes(HeaderBuffer.GetData(), HeaderSize))
		return false;

	//ID 와 Size추출
	FPacketHeader Header;
	{
		//HeaderBuffer에 있는 데이터를  Header로 꺼내오기
		FMemoryReader Reader(HeaderBuffer);
		Reader << Header;
		//MSG_LOG_TEMP(TEXT("Recv PacketID : %d, PacketSize : %d"), Header.PacketID, Header.PacketSize);
	}

	//ID 와 Size복사
	_OutPacket = HeaderBuffer;

	//패킷 내용 파싱
	TArray<uint8> PayloadBuffer;
	const int32 PayLoadSize = Header.PacketSize - HeaderSize;
	
	//패킷 아이디만 가지고 처리하고,
	// 패킷 헤더 이외엔 따로 정보가 없는 패킷인 경우엔 바로 끝내기
	if (0 == PayLoadSize)
		return true;

	_OutPacket.AddZeroed(PayLoadSize);	//추가 영역 할당

	if (true == ReceiveDesiredBytes(&_OutPacket[HeaderSize], PayLoadSize))
		return true;

	return false;
}

bool RecvWorker::ReceiveDesiredBytes(uint8* _Result, int32 _Size)
{
	//수신받은 데이터가 없거나, 접속이 종료되었다면 return false
	uint32 PendingDataSize;
	if (false == Socket->HasPendingData(PendingDataSize) || PendingDataSize <= 0)
		return false;

	uint32 Offset = 0;
	while (0 < _Size)
	{
		//NumRead : 얼만큼 수신받았는지
		int32 NumRead = 0;

		//수신받은 데이터를 _Result + Offset에 _Size크기만큼 복사
		//TCP답게 _Size크기만큼 데이터가 안 들어올 수 있음
		//그 예외처리는 아래에서 확인
		Socket->Recv(_Result + Offset, _Size, OUT NumRead);

		//읽어들인 값이 예상한 값보다 큰 경우는 말이 안됨(예외처리)
		check(NumRead <= _Size);

		//종료되는 경우(예외처리)
		if (NumRead <= 0)
			return false;

		//다음 while문을 위해 Offset과 _Size 위치/크기 조정
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

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FSocket;

/*---------------------------------------------
							FPacketHeader
---------------------------------------------*/

struct MABINOGI_API FPacketHeader
{
	FPacketHeader()
		:PacketSize(0), PacketID(0)
	{
		
	}

	FPacketHeader(uint16 _PacketSize,uint16 _PacketID)
		:PacketSize(_PacketSize), PacketID(_PacketID)
	{

	}

	friend FArchive& operator << (FArchive& _Ar, FPacketHeader& _Header)
	{
		_Ar << _Header.PacketSize;
		_Ar << _Header.PacketID;
		return _Ar;
	}

	uint16 PacketSize;
	uint16 PacketID;
};


/*---------------------------------------------
							RecvWorker
---------------------------------------------*/

/**
 * 언리얼에서 제공하는 스레드방식으로 네트워크 수신을 담당한다
 */
class MABINOGI_API RecvWorker : public FRunnable
{
public:
	RecvWorker(FSocket* _Socket, TSharedPtr<class PacketSession> _Session);
	~RecvWorker();

	//아래 가상함수들은 스레드가 생성되면 자동으로 실행됨
	virtual bool Init() override;
	virtual uint32 Run() override;	
	virtual void Exit() override;

	void Destroy();

private:
	//데이터 수신받기
	bool ReceivePacket(TArray<uint8>& _OutPacket);

	//원하는 크기의 패킷이 올때까지 기다리고 결과물을 _Result에 뱉어준다
	bool ReceiveDesiredBytes(uint8* _Result, int32 _Size);

private:
	FRunnableThread* Thread = nullptr;
	bool bRunning = true;

	FSocket* Socket;
	TWeakPtr<class PacketSession> SessionRef;
};


/*---------------------------------------------
							SendWorker
---------------------------------------------*/

/**
 * 언리얼에서 제공하는 스레드방식으로 네트워크 송신을 담당한다
 */
class MABINOGI_API SendWorker : public FRunnable
{
public:
	SendWorker(FSocket* _Socket, TSharedPtr<class PacketSession> _Session);
	~SendWorker();

	//아래 가상함수들은 스레드가 생성되면 자동으로 실행됨
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy();
	
private:
	bool SendDesiredBytes(const uint8* _Buffer, int32 _Size);

private:
	FRunnableThread* Thread = nullptr;
	bool bRunning = true;

	FSocket* Socket;
	TWeakPtr<class PacketSession> SessionRef;
};


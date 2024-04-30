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
 * �𸮾󿡼� �����ϴ� ������������ ��Ʈ��ũ ������ ����Ѵ�
 */
class MABINOGI_API RecvWorker : public FRunnable
{
public:
	RecvWorker(FSocket* _Socket, TSharedPtr<class PacketSession> _Session);
	~RecvWorker();

	//�Ʒ� �����Լ����� �����尡 �����Ǹ� �ڵ����� �����
	virtual bool Init() override;
	virtual uint32 Run() override;	
	virtual void Exit() override;

	void Destroy();

private:
	//������ ���Źޱ�
	bool ReceivePacket(TArray<uint8>& _OutPacket);

	//���ϴ� ũ���� ��Ŷ�� �ö����� ��ٸ��� ������� _Result�� ����ش�
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
 * �𸮾󿡼� �����ϴ� ������������ ��Ʈ��ũ �۽��� ����Ѵ�
 */
class MABINOGI_API SendWorker : public FRunnable
{
public:
	SendWorker(FSocket* _Socket, TSharedPtr<class PacketSession> _Session);
	~SendWorker();

	//�Ʒ� �����Լ����� �����尡 �����Ǹ� �ڵ����� �����
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


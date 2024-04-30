// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

struct PacketHeader
{
	uint16 size;
	uint16 id;
};


/*----------------------------------
					SendBuffer
----------------------------------*/

/*
		��ε�ĳ��Ʈ �� ��
		���� �����Ϳ� ���� ���縦 ���̱� ����
		shared_ptr�� ����(�淮����)

		����� ����������(SendBufferPool ���� ��)
		�����͸� ��� ������ ���� �� SendBuffer�� �Ҹ��
*/
class SendBuffer : public TSharedFromThis<SendBuffer>
{
public:
	SendBuffer(int32 bufferSize);
	~SendBuffer();

	SendBuffer(const SendBuffer& _Other) = delete;
	SendBuffer(SendBuffer&& _Other) noexcept = delete;
	SendBuffer& operator=(const SendBuffer& _Other) = delete;
	SendBuffer& operator=(const SendBuffer&& _Other) noexcept = delete;

	//���� ������� ���� ù �ּ� ��ȯ
	BYTE* Buffer() { return _buffer.GetData(); }
	//�� ���ۿ� ���� ������ ũ��
	uint32 WriteSize() { return _writeSize; }
	int32 Capacity() { return static_cast<int32>(_buffer.Num()); }

	void CopyData(void* data, int32 len);

	//�ش� ���� ������ ������ �ۼ�
	void Close(uint32 writeSize);

private:
	TArray<BYTE>				_buffer;
	uint32							_writeSize = 0;
};

#define USING_SHARED_PTR(name) using name##Ref = TSharedPtr<class name>
USING_SHARED_PTR(Session);
USING_SHARED_PTR(PacketSession);
USING_SHARED_PTR(SendBuffer);

#include "ServerPacketHandler.h"
#include "MabiGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#define SEND_PACKET(pkt)																												\
				SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);						\
				Cast<UMabiGameInstance>(GWorld->GetGameInstance())->SendPacket(sendBuffer);


#define SCREEN_ERROR_MSG(_ErrorMsg)	\
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(_ErrorMsg)));

#define GET_GAME_INSTANCE(__Wolrd)												\
				Cast<UMabiGameInstance>(__Wolrd->GetGameInstance())	\
				
				

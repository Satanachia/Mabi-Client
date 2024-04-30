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
		브로드캐스트 할 때
		같은 데이터에 대한 복사를 줄이기 위해
		shared_ptr로 관리(경량패턴)

		참고로 아직까지는(SendBufferPool 적용 전)
		데이터를 모두 보내고 나면 이 SendBuffer는 소멸됨
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

	//실제 사용중인 버퍼 첫 주소 반환
	BYTE* Buffer() { return _buffer.GetData(); }
	//이 버퍼에 적은 데이터 크기
	uint32 WriteSize() { return _writeSize; }
	int32 Capacity() { return static_cast<int32>(_buffer.Num()); }

	void CopyData(void* data, int32 len);

	//해당 버퍼 공간에 데이터 작성
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
				
				

// Copyright Epic Games, Inc. All Rights Reserved.

#include "Mabinogi.h"
#include "Modules/ModuleManager.h"


IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Mabinogi, "Mabinogi" );

SendBuffer::SendBuffer(int32 bufferSize)
{
	_buffer.SetNum(bufferSize);
}

SendBuffer::~SendBuffer()
{

}

void SendBuffer::CopyData(void* data, int32 len)
{
	::memcpy(_buffer.GetData(), data, len);
	_writeSize = len;
}

//�ش� ���� ������ ������ �ۼ�
void SendBuffer::Close(uint32 writeSize)
{
	_writeSize = writeSize;
}



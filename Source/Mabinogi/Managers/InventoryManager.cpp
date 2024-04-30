// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/InventoryManager.h"
#include "UI/HUD/InvenItemWidget.h"
#include "Components/GridPanel.h"
#include "Mabinogi.h"
#include "UI/HUD/HUDWidget.h"
#include "UI/HUD/ActionSlotWidget.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "Actors/Network/NetworkPlayer.h"
#include "Managers/ResourceManager.h"

InventoryManager::InventoryManager()
{
	
}

InventoryManager::~InventoryManager()
{

}

void InventoryManager::Init(TArray<UInvenItemWidget*>&& _Widgets)
{
	Widgets = std::move(_Widgets);
	int32 Size = Widgets.Num();
	if (false == Datas.IsEmpty())
	{
		check(Size == Datas.Num());
		for (int32 i = 0; i < Size; ++i)
		{
			Alter(i);
		}
		return;
	}
	
	Datas.SetNum(Size);
	for (Protocol::ItemInfo& Data : Datas)
	{
		Data.set_type(Protocol::ITEM_TYPE_NONE);
		Data.set_count(0);
	}
}

void InventoryManager::Handle_Init_Inventory(Protocol::S_INIT_INVENTORY& _Pkt)
{
	for (int32 i = 0; i < _Pkt.items_size(); ++i)
	{
		const Protocol::ItemInfo& ItemInfo = _Pkt.items(i);
		Protocol::ItemType ItemType = ItemInfo.type();
		int32 ItemCount = ItemInfo.count();
		
		check(false == ServerItemDatas.Contains(ItemType));
		ServerItemDatas.Add(ItemType, ItemCount);
	}
}

void InventoryManager::LoadInventory(ANetworkPlayer* _Player)
{
	if (true == ServerItemDatas.IsEmpty())
		return;

	FString Path;
	GetPlayerFilePath(OUT Path, _Player);
	if (false == FPaths::FileExists(Path))
	{
		MoveServerItemInfoes();
		return;
	}
	
	FArchive* FileReaderAr = IFileManager::Get().CreateFileReader(*Path);
	check(FileReaderAr);
	for (int32 i = 0; i < Datas.Num(); ++i)
	{
		int32 ClientItemType = -1;
		int32 ClientItemCount = -1;

		*FileReaderAr << ClientItemType;
		*FileReaderAr << ClientItemCount;
		
		check(0 <= ClientItemType);
		check(0 <= ClientItemCount);
		Protocol::ItemType ItemType = static_cast<Protocol::ItemType>(ClientItemType);
		int32* FindIter = ServerItemDatas.Find(ItemType);
		if (nullptr == FindIter)
			continue;

		int32& ServerItemCount = *FindIter;
		if (ServerItemCount < ClientItemCount)
		{
			ClientItemCount = ServerItemCount;
		}

		ServerItemCount -= ClientItemCount;
		if (0 == ServerItemCount)
		{
			ServerItemDatas.Remove(ItemType);
		}

		Datas[i].set_type(ItemType);
		Datas[i].set_count(ClientItemCount);
		Alter(i);
	}
	FileReaderAr->Close();
	delete FileReaderAr;
	FileReaderAr = nullptr;

	MoveServerItemInfoes();
}

void InventoryManager::SaveInventory(ANetworkPlayer* _Player)
{
	FString Path;
	GetPlayerFilePath(OUT Path, _Player);

	FArchive* FileWriterAr = IFileManager::Get().CreateFileWriter(*Path);
	check(FileWriterAr);

	for (int32 i = 0; i < Datas.Num(); ++i)
	{
		int32 ItemType = static_cast<int32>(Datas[i].type());
		int32  ItemCount = Datas[i].count();

		*FileWriterAr << ItemType;
		*FileWriterAr << ItemCount;
	}
	
	FileWriterAr->Close();
	delete FileWriterAr;
	FileWriterAr = nullptr;
}


void InventoryManager::GetPlayerFilePath(OUT FString& _Path, ANetworkPlayer* _Player)
{
	check(_Player);
	
	FString PlayerName = UTF8_TO_TCHAR(_Player->GetProtocolName().c_str());
	_Path = FPaths::Combine(FPlatformMisc::ProjectDir(), TEXT("Saved"));
	_Path = FPaths::Combine(_Path, TEXT("Inventory"));
	_Path = FPaths::Combine(*_Path, *PlayerName);
	FPaths::MakeStandardFilename(_Path);
}

void InventoryManager::MoveServerItemInfoes()
{
	if (true == ServerItemDatas.IsEmpty())
		return;

	for (const TPair<Protocol::ItemType, int32>& Data : ServerItemDatas)
	{
		Protocol::ItemInfo ItemInfo;
		ItemInfo.set_type(Data.Key);
		ItemInfo.set_count(Data.Value);
		AcquireItem(ItemInfo);
	}
	ServerItemDatas.Empty();
}


void InventoryManager::AcquireItem(const Protocol::ItemInfo& _Item, bool _IsSound /*= false*/)
{
	Protocol::ItemType Type = _Item.type();
	int32 AddCount = _Item.count();
	
	int32 Index = FindAddIndex(Type);
	if (Index < 0)
	{
		//인벤토리 공간 부족
		return;
	}
	
	Protocol::ItemInfo& Data = Datas[Index];
	int32 AlterCount = Data.count() + AddCount;
	Data.set_type(Type);
	Data.set_count(AlterCount);
	Alter(Index);

	if (true == _IsSound)
	{
		ResourceManager::PlaySound(SoundType::EatItem, Widgets[Index]->GetWorld());
	}
}

void InventoryManager::MoveItem(UInvenItemWidget* _From, UInvenItemWidget* _To)
{
	check(_From);
	check(_To);

	UActionSlotWidget* FromActionSlot = _From->FindBindingSlot();

	{
		int32 FromIndex = _From->GetInventoryIndex();
		int32 ToIndex = _To->GetInventoryIndex();

		check(IsValidIndex(FromIndex));
		check(IsValidIndex(ToIndex));
		check(false == IsEmpty(FromIndex));
		check(true == IsEmpty(ToIndex));

		Datas[ToIndex].CopyFrom(Datas[FromIndex]);
		Datas[FromIndex].set_type(Protocol::ITEM_TYPE_NONE);
		Datas[FromIndex].set_count(0);

		Alter(FromIndex);
		Alter(ToIndex);
	}
	
	if (nullptr == FromActionSlot)
		return;

	FromActionSlot->Copy(_To);
}

void InventoryManager::SwapItem(UInvenItemWidget* _Left, UInvenItemWidget* _Right)
{
	check(_Left);
	check(_Right);

	UActionSlotWidget* LeftBindingSlot = _Left->FindBindingSlot();
	UActionSlotWidget* RightBindingSlot = _Right->FindBindingSlot();
	
	{
		int32 LeftIndex = _Left->GetInventoryIndex();
		int32 RightIndex = _Right->GetInventoryIndex();

		check(IsValidIndex(LeftIndex));
		check(IsValidIndex(RightIndex));
		check(false == IsEmpty(LeftIndex));
		check(false == IsEmpty(RightIndex));

		Protocol::ItemInfo Temp;
		Protocol::ItemInfo& LeftData = Datas[LeftIndex];
		Protocol::ItemInfo& RightData = Datas[RightIndex];

		Temp.CopyFrom(LeftData);
		LeftData.CopyFrom(RightData);
		RightData.CopyFrom(Temp);

		Alter(LeftIndex);
		Alter(RightIndex);
	}

	if (nullptr != LeftBindingSlot)
	{
		LeftBindingSlot->Copy(_Right);
	}

	if (nullptr != RightBindingSlot)
	{
		RightBindingSlot->Copy(_Left);
	}
}

void InventoryManager::DropItem(UInvenItemWidget* _Widget)
{
	int32 Index = _Widget->GetInventoryIndex();
	check(IsValidIndex(Index));
	
	if (true == IsEmpty(Index))
		return;

	Protocol::C_DROP_ITEM DropPkt;
	Protocol::ItemInfo* ItemInfo = DropPkt.mutable_item();
	ItemInfo->CopyFrom(Datas[Index]);
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(DropPkt);
	UMabiGameInstance* GameInst = Cast<UMabiGameInstance>(_Widget->GetGameInstance());
	check(GameInst);
	GameInst->SendPacket(sendBuffer);
	
	Datas[Index].set_type(Protocol::ITEM_TYPE_NONE);
	Datas[Index].set_count(0);
	Alter(Index);

	ResourceManager::PlaySound(SoundType::DropItem, _Widget->GetWorld());
}

int32 InventoryManager::FindAddIndex(Protocol::ItemType _InsertItemType)
{
	int32 EmptyIndex = -1;
	
	for (int32 i = 0; i < Datas.Num(); ++i)
	{
		const Protocol::ItemInfo& Data = Datas[i];;
		Protocol::ItemType FindItemType = Data.type();

		if (_InsertItemType == FindItemType)
			return i;

		if ((EmptyIndex < 0) && (Protocol::ITEM_TYPE_NONE == FindItemType))
		{
			EmptyIndex = i;
		}
	}

	return EmptyIndex;
}

void InventoryManager::Alter(int32 _Index)
{
	check(IsValidIndex(_Index));

	const Protocol::ItemInfo& Data = Datas[_Index];
	UInvenItemWidget* Widget = Widgets[_Index];
	Widget->Alter(Data);
}

bool InventoryManager::IsValidIndex(int32 _Index)
{
	if (_Index < 0)
		return false;

	if (Datas.Num() <= _Index)
		return false;

	if (Widgets.Num() <= _Index)
		return false;

	return true;
}



bool InventoryManager::IsEmpty(int32 _Index)
{
	check(IsValidIndex(_Index));

	const Protocol::ItemInfo& Data= Datas[_Index];

	bool Empty = true;
	Empty &= (Data.type() == Protocol::ITEM_TYPE_NONE);
	Empty &= (Data.count() == 0);
	return Empty;
}

void InventoryManager::UseItem(UInvenItemWidget* _Widget)
{
	check(_Widget);
	int32 Index = _Widget->GetInventoryIndex();
	check(IsValidIndex(Index));
	check(false == IsEmpty(Index));

	Protocol::ItemInfo& Data = Datas[Index];
	Protocol::ItemType ItemType = Data.type();
	int32 ItemCount = Data.count();
	--ItemCount;
	Data.set_count(ItemCount);
	if (0 == ItemCount)
	{
		Data.set_type(Protocol::ITEM_TYPE_NONE);
	}

	Alter(Index);
	UActionSlotWidget* ActionSlot = _Widget->FindBindingSlot();
	if (nullptr != ActionSlot)
	{
		USlotWidget* Widget = IsEmpty(Index) ? nullptr : _Widget;
		ActionSlot->Copy(Widget);
	}

	Protocol::C_USE_ITEM Pkt;
	Pkt.set_item(ItemType);
	SEND_PACKET(Pkt);

	ResourceManager::PlaySound(SoundType::UseItem, _Widget->GetWorld());
}



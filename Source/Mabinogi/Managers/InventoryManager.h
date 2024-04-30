// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Protocol.pb.h"

class UInventoryWidget;
class UInvenItemWidget;
class ANetworkPlayer;

class MABINOGI_API InventoryManager
{
public:
	InventoryManager();
	~InventoryManager();

	void Init(TArray<UInvenItemWidget*>&& _Widgets);
	void Handle_Init_Inventory(Protocol::S_INIT_INVENTORY& _Pkt);
	void LoadInventory(ANetworkPlayer* _Player);
	void SaveInventory(ANetworkPlayer* _Player);

	void AcquireItem(const Protocol::ItemInfo& _Item, bool _IsSound = false);
	void MoveItem(UInvenItemWidget* _From, UInvenItemWidget* _To);
	void SwapItem(UInvenItemWidget* _Left, UInvenItemWidget* _Right);
	void DropItem(UInvenItemWidget* _Widget);
	bool IsEmpty(int32 _Index);
	void UseItem(UInvenItemWidget* _Widget);

	
private:
	int32 FindAddIndex(Protocol::ItemType _InsertItemType);
	void Alter(int32 _Index);
	bool IsValidIndex(int32 _Index);
	void GetPlayerFilePath(OUT FString& _Path, ANetworkPlayer* _Player);
	void MoveServerItemInfoes();

private:
	TArray<UInvenItemWidget*> Widgets;
	TArray<Protocol::ItemInfo> Datas;
	TMap<Protocol::ItemType, int32> ServerItemDatas;
};

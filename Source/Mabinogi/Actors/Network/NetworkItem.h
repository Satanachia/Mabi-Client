// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Network/NetworkObject.h"
#include "Interface/LClickInterface.h"
#include "NetworkItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class MABINOGI_API ANetworkItem : public ANetworkObject, public ILClickInterface
{
	GENERATED_BODY()
	
public:
	ANetworkItem();

protected:
	virtual void InitFromType(uint32 _NetworkSecondType) override;
	virtual void Tick(float _DeltaTime) override;
	virtual void ExcuteLClick(ANetworkPlayerController* _PlayerCtrl) override;

private:
	void SetMaterial(Protocol::ItemType _Type);
	void Move(float _DeltaTime);
	void Look();
	bool IsCollision(AActor* _Target);
	void Excute(ANetworkPlayerController* _PlayerCtrl);

private:
	UStaticMeshComponent* Mesh = nullptr;
	
	Protocol::ItemInfo ItemInfo;
	const float CollisionRadius = 100.f;
	const float HalfHeight = 30.f;
	float Timer = 0.f;
	bool IsUsed = false;
};

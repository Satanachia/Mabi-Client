// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Protocol.pb.h"
#include "LobbyWatiActor.generated.h"


class USkeletalMesh;
class UAnimationAsset;

UCLASS()
class MABINOGI_API ALobbyWatiActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ALobbyWatiActor();

	void ChangeMesh(Protocol::CreatureType _type);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Mesh;
	
	TMap<Protocol::CreatureType, USkeletalMesh*> MeshRes;
	TMap<Protocol::CreatureType, UAnimationAsset*> AnimInstClass;
};

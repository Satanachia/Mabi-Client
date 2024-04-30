// Fill out your copyright notice in the Description page of Project Settings.


#include "Tool/MonsterSpawnerToolComponent.h"

UMonsterSpawnerToolComponent::UMonsterSpawnerToolComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMonsterSpawnerToolComponent::Load(FArchive* _FileReaderAr)
{
	FVector ThisLocation = LoadLocation(_FileReaderAr);
	SetWorldLocation(ThisLocation);

	int32 DestCount = 0;
	*_FileReaderAr << DestCount;
	for (int32 i = 0; i < DestCount; ++i)
	{
		FString DestName = TEXT("Destination") + FString::FromInt(i);
		/*USceneComponent* Destination  = CreateDefaultSubobject<USceneComponent>(FName(*DestName));
		Destination->SetupAttachment(this);*/

		USceneComponent* Destination = NewObject<USceneComponent>(this, FName(*DestName));
		Destination->RegisterComponent();
		Destination->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

		FVector DestLocation = LoadLocation(_FileReaderAr);
		Destination->SetWorldLocation(DestLocation);
	}
}


void UMonsterSpawnerToolComponent::Save(FArchive* _FileWriterAr)
{
	SaveLocation(GetComponentLocation(), _FileWriterAr);

	const TArray<USceneComponent*>& Children = GetAttachChildren();
	int32 DestCount = Children.Num();
	*_FileWriterAr << DestCount;
	for (USceneComponent* Child : Children)
	{
		SaveLocation(Child->GetComponentLocation(), _FileWriterAr);
	}
}

void UMonsterSpawnerToolComponent::DebugRender()
{
	DrawDebugCapsule(GetWorld(), GetComponentLocation(), 88.f, 34.f, FQuat::Identity, FColor::Magenta, false, 0.f);

	const TArray<USceneComponent*>& Children = GetAttachChildren();
	for (USceneComponent* Child : Children)
	{
		FVector ChildLocation = Child->GetComponentLocation();
		DrawDebugLine(GetWorld(), GetComponentLocation(), ChildLocation, FColor::Magenta, false, 0.f);
	}
}


FVector UMonsterSpawnerToolComponent::LoadLocation(FArchive* _FileReaderAr)
{
	float X = 0.f;
	float Y = 0.f;
	float Z = 0.f;

	*_FileReaderAr << X;
	*_FileReaderAr << Y;
	*_FileReaderAr << Z;
	
	return FVector(X, Y, Z);
}



void UMonsterSpawnerToolComponent::SaveLocation(const FVector& _Location, FArchive* _FileWriterAr)
{
	float X = _Location.X;
	float Y = _Location.Y;
	float Z = _Location.Z;
	
	*_FileWriterAr << X;
	*_FileWriterAr << Y;
	*_FileWriterAr << Z;
}


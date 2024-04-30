// Fill out your copyright notice in the Description page of Project Settings.


#include "Tool/MonsterSpawnerToolHandler.h"
#include "Tool/MonsterSpawnerToolComponent.h"
#include "Levels/BattleLevel.h"

AMonsterSpawnerToolHandler::AMonsterSpawnerToolHandler()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	FolderPath = FPaths::Combine(FPlatformMisc::ProjectDir(), TEXT(".."));
	FolderPath = FPaths::Combine(*FolderPath, TEXT("Server"));
	FolderPath = FPaths::Combine(*FolderPath, TEXT("Common"));
	FolderPath = FPaths::Combine(*FolderPath, TEXT("MonsterSpawnPositions"));
}


void AMonsterSpawnerToolHandler::BeginPlay()
{
	Super::BeginPlay();
	Load();
}

void AMonsterSpawnerToolHandler::Load()
{
	if (nullptr == Level)
		return;

	//ÆÄÀÏ Å½»ö
	check(Level);
	FString LevelName = Level->GetName();
	FString Path = FPaths::Combine(*FolderPath, *LevelName);
	FPaths::MakeStandardFilename(Path);
	if (false == FPaths::FileExists(Path))
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Magenta, FString::Printf(TEXT("Not Exists File!")));
		return;
	}


	//ÆÄÀÏ ¿ÀÇÂ(READ)
	FArchive* FileReaderAr = IFileManager::Get().CreateFileReader(*Path);
	check(FileReaderAr);
	
	int32 LevelTypeInt = 0;
	*FileReaderAr << LevelTypeInt;
	Protocol::LevelType LevelType = static_cast<Protocol::LevelType>(LevelTypeInt);

	//ÄÄÆ÷³ÍÆ® °¹¼öÈ®ÀÎ
	int32 PivotCount = 0;
	*FileReaderAr << PivotCount;

	for (int32 i = 0; i < PivotCount; ++i)
	{
		UMonsterSpawnerToolComponent* Pivot = CreatePivot(i);
		Pivot->Load(FileReaderAr);
	}
	
	FileReaderAr->Close();
	delete FileReaderAr;
	FileReaderAr = nullptr;
}


void AMonsterSpawnerToolHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TArray<UMonsterSpawnerToolComponent*> SpawnPiviots;
	GetSpawnerComponents(OUT SpawnPiviots);
	for (UMonsterSpawnerToolComponent* SpawnPiviot : SpawnPiviots)
	{
		SpawnPiviot->DebugRender();
	}

	Timer += DeltaTime;
	if (Timer < Duration)
		return;
	
	if (true == SaveBtn)
	{
		SaveBtn = false;
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Magenta, FString::Printf(TEXT("Save!")));
		Save();
	}
	
	GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Magenta, FString::Printf(TEXT("Tool Tick!")));
	Timer = 0.f;
}


void AMonsterSpawnerToolHandler::Save()
{
	if(nullptr == Level)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Monster Spawner Tool Error : Level Null")));
		return;
	}

	FString LevelName = Level->GetName();
	FString Path = FPaths::Combine(*FolderPath, *LevelName);
	FPaths::MakeStandardFilename(Path);
	if (false == FPaths::FileExists(Path))
	{
		check(FFileHelper::SaveStringToFile(TEXT(" "), *Path));
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Magenta, FString::Printf(TEXT("Create File! Path : %s"), *Path));
	}
	
	TArray<UMonsterSpawnerToolComponent*> SpawnPiviots;
	GetSpawnerComponents(OUT SpawnPiviots);

	//ÆÄÀÏ¿ÀÇÂ(WRITE)
	FArchive* FileWriterAr = IFileManager::Get().CreateFileWriter(*Path);
	check(FileWriterAr);

	
	Protocol::LevelType LevelType = Level.GetDefaultObject()->GetLevelType();
	int32 LevelTypeInt = static_cast<int32>(LevelType);
	*FileWriterAr << LevelTypeInt;

	
	int32 PivotCount = SpawnPiviots.Num();
	*FileWriterAr << PivotCount;
	for (UMonsterSpawnerToolComponent* Pivot : SpawnPiviots)
	{
		Pivot->Save(FileWriterAr);
	}
	
	FileWriterAr->Close();
	delete FileWriterAr;
	FileWriterAr = nullptr;
}

UMonsterSpawnerToolComponent* AMonsterSpawnerToolHandler::CreatePivot(int32 _Index)
{
	FString PivotName = TEXT("Pivot") + FString::FromInt(_Index);

	UMonsterSpawnerToolComponent* Pivot = nullptr;
	/*Pivot = CreateDefaultSubobject<UMonsterSpawnerToolComponent>(FName(*PivotName));
	Pivot->SetupAttachment(RootComponent);*/
	Pivot = NewObject<UMonsterSpawnerToolComponent>(this, FName(*PivotName));
	Pivot->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//AddOwnedComponent(Pivot);
	
	return Pivot;
}

void AMonsterSpawnerToolHandler::GetSpawnerComponents(OUT TArray<UMonsterSpawnerToolComponent*>& _Spanwers)
{
	const TSet<UActorComponent*>& Childred = GetComponents();
	_Spanwers.Empty();
	_Spanwers.Reserve(Childred.Num());
	for (UActorComponent* Child : Childred)
	{
		UMonsterSpawnerToolComponent* SpawnPivot = Cast<UMonsterSpawnerToolComponent>(Child);
		if (nullptr == SpawnPivot)
			continue;

		_Spanwers.Add(SpawnPivot);
	}
}

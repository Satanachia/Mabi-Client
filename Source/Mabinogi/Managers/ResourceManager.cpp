// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/ResourceManager.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "MabiGameInstance.h"
#include "Actors/Network/NetworkPlayer.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

UTexture2D* ResourceManager::GetTexture(Protocol::ItemType _Key)
{
	static TMap<Protocol::ItemType, FString> Pathes;
	if (true == Pathes.IsEmpty())
	{
		Pathes.Add(Protocol::ITEM_TYPE_HP_PORTION, TEXT("/Script/Engine.Texture2D'/Game/Market/MyResource/UI/HpPortion.HpPortion'"));
		Pathes.Add(Protocol::ITEM_TYPE_LARGE_HP_PORTION, TEXT("/Script/Engine.Texture2D'/Game/Market/MyResource/UI/LargeHpPortion.LargeHpPortion'"));
	}
	
	return GetResource<Protocol::ItemType, UTexture2D>(_Key, Pathes, [](const FString& _Path)
	{
		return LoadObject<UTexture2D>(nullptr, *_Path);
	});
}

UTexture2D* ResourceManager::GetTexture(Protocol::AttackType _Key)
{
	static TMap<Protocol::AttackType, FString> Pathes;
	if (true == Pathes.IsEmpty())
	{
		Pathes.Add(Protocol::ATTACK_TYPE_SKILL_MAGE, TEXT("/Script/Engine.Texture2D'/Game/Market/MyResource/UI/MageSkill.MageSkill'"));
		Pathes.Add(Protocol::ATTACK_TYPE_SKILL_WARRIOR, TEXT("/Script/Engine.Texture2D'/Game/Market/MyResource/UI/WarriorSkill.WarriorSkill'"));
		Pathes.Add(Protocol::ATTACK_TYPE_SKILL_ROG, TEXT("/Script/Engine.Texture2D'/Game/Market/MyResource/UI/RogSkill.RogSkill'"));
	}
	
	return GetResource<Protocol::AttackType, UTexture2D>(_Key, Pathes, [](const FString& _Path)
	{
		return LoadObject<UTexture2D>(nullptr, *_Path);
	});
}

UMaterialInterface* ResourceManager::GetMaterial(Protocol::ItemType _Key)
{
	static TMap<Protocol::ItemType, FString> Pathes;
	if (true == Pathes.IsEmpty())
	{
		Pathes.Add(Protocol::ITEM_TYPE_HP_PORTION, TEXT("/Script/Engine.Material'/Game/Market/MyResource/Material/HpPortion_Mat.HpPortion_Mat'"));
		Pathes.Add(Protocol::ITEM_TYPE_LARGE_HP_PORTION, TEXT("/Script/Engine.Material'/Game/Market/MyResource/Material/LargeHpPortion_Mat.LargeHpPortion_Mat'"));
	}

	return GetResource<Protocol::ItemType, UMaterialInterface>(_Key, Pathes, [](const FString& _Path)
	{
		FSoftObjectPath MaterialPath(_Path);
		UMaterialInterface* LoadedMaterial = nullptr;
		while (nullptr == LoadedMaterial)
		{
			LoadedMaterial = Cast<UMaterialInterface>(MaterialPath.TryLoad());
		}

		return LoadedMaterial;
	});
}

void ResourceManager::PlaySound(SoundType _Type, UWorld* _World)
{
	static TMap<SoundType, FString> Pathes;
	if (true == Pathes.IsEmpty())
	{
		Pathes.Add(SoundType::AttackSwing, TEXT("/Script/Engine.SoundWave'/Game/Market/MyResource/Sounds/SFX/AttackSwing.AttackSwing'"));
		Pathes.Add(SoundType::ChickAttack, TEXT("/Script/Engine.SoundWave'/Game/Market/MyResource/Sounds/SFX/ChickenAttack.ChickenAttack'"));
		Pathes.Add(SoundType::ChickDamaged, TEXT("/Script/Engine.SoundWave'/Game/Market/MyResource/Sounds/SFX/ChickDamaged.ChickDamaged'"));
		Pathes.Add(SoundType::ChickDeath ,TEXT("/Script/Engine.SoundWave'/Game/Market/MyResource/Sounds/SFX/ChickDeath.ChickDeath'"));
		Pathes.Add(SoundType::ChickIdle, TEXT("/Script/Engine.SoundWave'/Game/Market/MyResource/Sounds/SFX/ChickIdle.ChickIdle'"));
		Pathes.Add(SoundType::Coin, TEXT("/Script/Engine.SoundWave'/Game/Market/MyResource/Sounds/SFX/Coin.Coin'"));
		Pathes.Add(SoundType::DropItem, TEXT("/Script/Engine.SoundWave'/Game/Market/MyResource/Sounds/SFX/DropItem.DropItem'"));
		Pathes.Add(SoundType::EatItem, TEXT("/Script/Engine.SoundWave'/Game/Market/MyResource/Sounds/SFX/EatItem.EatItem'"));
		Pathes.Add(SoundType::Hit, TEXT("/Script/Engine.SoundWave'/Game/Market/MyResource/Sounds/SFX/Hit.Hit'"));
		Pathes.Add(SoundType::MageSkill, TEXT("/Script/Engine.SoundWave'/Game/Market/MyResource/Sounds/SFX/MageSkill.MageSkill'"));
		Pathes.Add(SoundType::MageSkillReady, TEXT("/Script/Engine.SoundWave'/Game/Market/MyResource/Sounds/SFX/MageSkillReady.MageSkillReady'"));
		Pathes.Add(SoundType::Portal, TEXT("/Script/Engine.SoundWave'/Game/Market/MyResource/Sounds/SFX/Portal.Portal'"));
		Pathes.Add(SoundType::ThinkDone, TEXT("/Script/Engine.SoundWave'/Game/Market/MyResource/Sounds/SFX/ThinkDone.ThinkDone'"));
		Pathes.Add(SoundType::ThinkReady, TEXT("/Script/Engine.SoundWave'/Game/Market/MyResource/Sounds/SFX/ThinkReady.ThinkReady'"));
		Pathes.Add(SoundType::UseItem, TEXT("/Script/Engine.SoundWave'/Game/Market/MyResource/Sounds/SFX/UseItem.UseItem'"));
		Pathes.Add(SoundType::WarriorThrow, TEXT("/Script/Engine.SoundWave'/Game/Market/MyResource/Sounds/SFX/WarriorThrow.WarriorThrow'"));
	}
	
	USoundBase* Sound = nullptr;
	Sound = GetResource<SoundType, USoundBase>(_Type, Pathes, [](const FString& _Path)
	{
		return LoadObject<USoundBase>(nullptr, *_Path);
	});
	
	check(_World);
	UGameplayStatics::PlaySound2D(_World, Sound);
}

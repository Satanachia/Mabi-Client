// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Protocol.pb.h"


class UTexture2D;
class UMaterialInterface;

enum class SoundType
{
	AttackSwing,		/*OK*/
	Hit,						/*OK*/
	Coin,					/*OK*/

	ChickAttack,		/*OK*/
	ChickDamaged,	/*OK*/
	ChickDeath,			/*OK*/
	ChickIdle,			/*OK*/

	DropItem,			/*OK*/
	EatItem,				/*OK*/
	UseItem,				/*OK*/
	Portal,					/*OK*/

	MageSkill,			/**/
	MageSkillReady,	/**/
	WarriorThrow,	/**/

	ThinkDone,			/*OK*/
	ThinkReady,			/*OK*/
};


class MABINOGI_API ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static UTexture2D* GetTexture(Protocol::ItemType _Key);
	static UTexture2D* GetTexture(Protocol::AttackType _Key);
	static UMaterialInterface* GetMaterial(Protocol::ItemType _Key);
	static void PlaySound(SoundType _Type, UWorld* _World);

private:
	template <typename KeyType, typename ResType>
	static ResType* GetResource(KeyType _Key, TMap<KeyType, FString>& _Pathes, const TFunction<ResType*(const FString&)>& _LoadProc)
	{
		static TMap<KeyType, ResType*> Dictionary;
		ResType** FindResIter = Dictionary.Find(_Key);
		if (nullptr != FindResIter)
			return *FindResIter;
		
		FString* FindPathIter = _Pathes.Find(_Key);
		check(FindPathIter);
		const FString& Path = *FindPathIter;
		check(false == Path.IsEmpty());
		
		check(_LoadProc);
		ResType* Result = _LoadProc(*Path);
		check(Result);
		
		Dictionary.Add(_Key, Result);
		return Result;
	}

};

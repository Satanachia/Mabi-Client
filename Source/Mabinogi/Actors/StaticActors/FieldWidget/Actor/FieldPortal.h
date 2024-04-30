// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/StaticActors/FieldWidget/Actor/FieldWidgetActorBase.h"
#include "FieldPortal.generated.h"

class ABattleLevel;
class UParticleSystemComponent;

/**
 * 
 */
UCLASS()
class MABINOGI_API AFieldPortal : public AFieldWidgetActorBase
{
	GENERATED_BODY()
	
public:
	AFieldPortal();

protected:
	virtual void PostInitializeComponents() override;

protected:
	virtual void Excute(ANetworkPlayerController* _PlayerCtrl) override;
	virtual void OnHoverLMouse(ANetworkPlayerController* _PlayerCtrl) override;
	virtual void OnUnHoverLMouse(ANetworkPlayerController* _PlayerCtrl) override;

public:
	UPROPERTY(EditAnywhere)
	FVector2D WidgetInitScale = FVector2D(100.f, 100.f);

private:
	UPROPERTY(EditAnywhere, meta = (allowprivateaccess = true))
	FVector NextLevelStartLocation;
	
	UPROPERTY(EditAnywhere, meta = (allowprivateaccess = true))
	TSubclassOf<ABattleLevel> NextLevelClass;
	
	UPROPERTY(EditAnywhere, meta = (allowprivateaccess = true))
	UParticleSystemComponent* ParticleSystemComponent = nullptr;

	UPROPERTY(EditAnywhere, meta = (allowprivateaccess = true))
	FString WidgetTextValue;
};

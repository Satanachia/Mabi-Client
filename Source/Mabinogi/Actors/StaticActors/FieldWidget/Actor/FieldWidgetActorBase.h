// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/LClickInterface.h"
#include "FieldWidgetActorBase.generated.h"

class UWidgetComponent;
class UUserWidget;
class UBoxComponent;
class ANetworkPlayerController;

UCLASS()
class MABINOGI_API AFieldWidgetActorBase : public AActor, public ILClickInterface
{
	GENERATED_BODY()
	
public:	
	AFieldWidgetActorBase();

protected:
	virtual void PostInitializeComponents() override;
	virtual void ExcuteLClick(ANetworkPlayerController* _PlayerCtrl) override;
	virtual void Excute(ANetworkPlayerController* _PlayerCtrl) PURE_VIRTUAL(AFieldWidgetActorBase::Excute, );
	
	UUserWidget* GetUserWidget();
	UWidgetComponent* GetFieldWidget() { return FieldWidget; }

private:
	bool IsCollision(AActor* _Target);

private:
	UPROPERTY(VisibleAnywhere, meta = (allowprivateaccess = true))
	UWidgetComponent* FieldWidget = nullptr;
	
	UPROPERTY(EditAnywhere, meta = (allowprivateaccess = true))
	UBoxComponent* BoxCollider;
};

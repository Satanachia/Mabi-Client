#pragma once
#include "FSM/BaseState.h"
#include "Protocol.pb.h"
#include "CoreMinimal.h"

class BaseSkillState : public BaseState
{
public:
	BaseSkillState();
	~BaseSkillState();
	
protected:
	virtual void Start() override;
	virtual void EnterState() override;
	
	virtual void OnInteractionCallBack(PlayerInputType _Type) final;
	virtual void OnAttackStart(const FVector& _AttackLocation){}
	virtual void ExcuteSkill(const FVector& _AttackLocation) abstract;
	
private:
	void AnimNotify_Attack();
	void AnimNotify_Finish();

private:
	bool IsUsedSkill = false;
	FVector AttackLocation = FVector::ZeroVector;
};

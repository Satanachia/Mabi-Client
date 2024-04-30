#pragma once
#include "FSM/Skill/BaseSkillState.h"

class MageSkillState : public BaseSkillState
{
public:
	MageSkillState();
	~MageSkillState();

protected:
	virtual void OnAttackStart(const FVector& _AttackLocation) override;
	virtual void ExcuteSkill(const FVector& _AttackLocation) override;

private:
	
};

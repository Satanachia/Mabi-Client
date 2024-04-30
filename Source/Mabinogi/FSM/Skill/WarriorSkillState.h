#pragma once

#include "FSM/Skill/BaseSkillState.h"

class ANetworkPlayer;

class WarriorSkillState : public BaseSkillState
{
public:
	WarriorSkillState();
	~WarriorSkillState();

protected:
	virtual void ExcuteSkill(const FVector& _AttackLocation) override;

private:
	FVector AttackPos;
};

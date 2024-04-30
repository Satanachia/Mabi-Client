#pragma once
#include "FSM/Skill/BaseSkillState.h"

class ANetworkPlayer;

class RogSkillState : public BaseSkillState
{
public:
	RogSkillState();
	~RogSkillState();

protected:
	virtual void Start() override;

	virtual void ExcuteSkill(const FVector& _AttackLocation) override;

private:
	void ExcuteBlink();
	void Move(ANetworkPlayer* _Player);
	void SendMovePacket(ANetworkPlayer* _Player);

private:
	FVector MoveDestination;
};

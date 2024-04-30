#include "FSM/Skill/WarriorSKillState.h"
#include "AnimInst/CreatureAnimInstance.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "Actors/Network/NetworkPlayer.h"
#include "Mabinogi.h"

WarriorSkillState::WarriorSkillState()
{
}

WarriorSkillState::~WarriorSkillState()
{
}

void WarriorSkillState::ExcuteSkill(const FVector& _AttackLocation)
{
	Protocol::C_ATTACK AttackPkt;
	Protocol::PosInfo* TargetPos = AttackPkt.mutable_target_pos();
	TargetPos->set_x(_AttackLocation.X);
	TargetPos->set_y(_AttackLocation.Y);
	AttackPkt.set_attack_type(Protocol::ATTACK_TYPE_SKILL_WARRIOR);
	SEND_PACKET(AttackPkt);
}

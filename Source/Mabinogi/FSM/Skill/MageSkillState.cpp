#include "FSM/Skill/MageSkillState.h"
#include "Mabinogi.h"


MageSkillState::MageSkillState()
{
	
}

MageSkillState::~MageSkillState()
{
	
}

void MageSkillState::OnAttackStart(const FVector& _AttackLocation)
{
	BaseSkillState::OnAttackStart(_AttackLocation);

	Protocol::C_SPAWN_PARTICLE Pkt;
	Pkt.set_particle_type(Protocol::PARTICLE_TYPE_MAGIC_ZONE);
	Protocol::PosInfo* AttackPos = Pkt.mutable_position();
	AttackPos->set_x(_AttackLocation.X);
	AttackPos->set_y(_AttackLocation.Y);
	AttackPos->set_z(_AttackLocation.Z);
	SEND_PACKET(Pkt);
}

void MageSkillState::ExcuteSkill(const FVector& _AttackLocation)
{
	Protocol::C_ATTACK AttackPkt;
	Protocol::PosInfo* TargetPos = AttackPkt.mutable_target_pos();
	TargetPos->set_x(_AttackLocation.X);
	TargetPos->set_y(_AttackLocation.Y);
	AttackPkt.set_attack_type(Protocol::ATTACK_TYPE_SKILL_MAGE);
	SEND_PACKET(AttackPkt);
}



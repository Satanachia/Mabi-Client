#include "FSM/Skill/BaseSkillState.h"
#include "AnimInst/PlayerAnimInstance.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "Actors/Network/NetworkPlayer.h"
#include "Mabinogi.h"
#include "UI/ObjectInfoWidget.h"

BaseSkillState::BaseSkillState()
{
	
}

BaseSkillState::~BaseSkillState()
{
}

void BaseSkillState::Start()
{
	BaseState::Start();
	
	UPlayerAnimInstance* AnimInst = GetPlayerAnimInst();
	AnimInst->BindAttackNotify(std::bind(&BaseSkillState::AnimNotify_Attack, this));
	AnimInst->BindMontageFinish(std::bind(&BaseSkillState::AnimNotify_Finish, this));

	BlockOutsideChange();
}

void BaseSkillState::EnterState()
{
	BaseState::EnterState();

	IsUsedSkill = false;
}

void BaseSkillState::OnInteractionCallBack(PlayerInputType _Type)
{
	BaseState::OnInteractionCallBack(_Type);

	if (PlayerInputType::INPUT_LEFT_CLICK != _Type)
		return;
	
	if (true == IsUsedSkill)
		return;
	
	AttackLocation = GetPlayerController()->GetMouseWorldHit().Location;
	OnAttackStart(AttackLocation);
	IsUsedSkill = true;

	{
		Protocol::C_LOOK LookPkt;
		Protocol::PosInfo* LookPos = LookPkt.mutable_look_pos();
		LookPos->set_x(AttackLocation.X);
		LookPos->set_y(AttackLocation.Y);
		SEND_PACKET(LookPkt);
	}

	{
		Protocol::C_ANIMATION AniPkt;
		AniPkt.set_ani_type(Protocol::ANI_TYPE_SKILL);
		SEND_PACKET(AniPkt);
	}
}

void BaseSkillState::AnimNotify_Attack()
{
	if (false == IsThisWorking())
		return;
	
	ExcuteSkill(AttackLocation);
}

void BaseSkillState::AnimNotify_Finish()
{
	if (false == IsThisWorking())
		return;

	ChangeState(PLAYER_STATE_IDLE);
}


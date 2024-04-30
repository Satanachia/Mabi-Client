#include "FSM/AttackState.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "AnimInst/PlayerAnimInstance.h"
#include "Actors/Network/NetworkOthers.h"
#include "Actors/Network/NetworkPlayer.h"
#include "Define.h"
#include "Mabinogi.h"
#include "Kismet/GameplayStatics.h"



void AttackState::Start()
{
	BaseState::Start();

	UPlayerAnimInstance* AnimInst = GetPlayerAnimInst();
	AnimInst->BindAttackNotify(std::bind(&AttackState::AnimNotify_Attack, this));
	AnimInst->BindMontageFinish(std::bind(&AttackState::AnimNotify_Finish, this));

	BlockOutsideChange({ PLAYER_STATE_SKILL });
}

void AttackState::EnterState()
{
	BaseState::EnterState();

	Target = GetPlayerController()->GetTarget();
	check(Target);
	LookTarget();
	
	ReserveCombo = MIN_COMBO;
	SendAnimationPacket();
}

void AttackState::LookTarget()
{
	FVector TargetLocation = Target->GetActorLocation();

	Protocol::C_LOOK Pkt;
	Protocol::PosInfo* LookPos = Pkt.mutable_look_pos();
	LookPos->set_x(TargetLocation.X);
	LookPos->set_y(TargetLocation.Y);
	SEND_PACKET(Pkt);
}

void AttackState::SendAnimationPacket()
{
	Protocol::C_ANIMATION aniPkt;
	aniPkt.set_ani_type(Protocol::ANI_TYPE_NORMAL_ATTACK);
	SEND_PACKET(aniPkt);
}




void AttackState::OnInteractionCallBack(PlayerInputType _Type)
{
	BaseState::OnInteractionCallBack(_Type);

	if (INPUT_LEFT_CLICK != _Type)
		return;

	int32 CurCombo = GetPlayerAnimInst()->GetCurrentAttackCombo();
	ReserveCombo = (CurCombo + 1) % MAX_COMBO;
}

void AttackState::AnimNotify_Attack()
{
	if (false == IsThisWorking())
		return;

	SendAttackPacket();
	GetPlayerController()->ShakeCamera();
}

void AttackState::SendAttackPacket()
{
	Protocol::C_ATTACK AttackPkt;
	AttackPkt.set_target_id(Target->GetProtocolID());
	AttackPkt.set_attack_type(Protocol::ATTACK_TYPE_NORMAL_ATTACK);
	SEND_PACKET(AttackPkt);
}

void AttackState::AnimNotify_Finish()
{
	if (false == IsThisWorking())
		return;
	
	if (false == Target->IsAlive())
	{
		ChangeState(PLAYER_STATE_IDLE);
		return;
	}

	int32 CurCombo = GetPlayerAnimInst()->GetCurrentAttackCombo();
	if (CurCombo == ReserveCombo)
	{
		ChangeState(PLAYER_STATE_IDLE);
		return;
	}
	
	SendAnimationPacket();
}


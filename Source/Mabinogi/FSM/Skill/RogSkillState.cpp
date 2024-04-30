#include "FSM/Skill/RogSkillState.h"
#include "AnimInst/CreatureAnimInstance.h"
#include "Actors/Controllers/NetworkPlayerController.h"
#include "Actors/Network/NetworkPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Mabinogi.h"

RogSkillState::RogSkillState()
{
	
}

RogSkillState::~RogSkillState()
{
}


void RogSkillState::Start()
{
	BaseSkillState::Start();

	//UCreatureAnimInstance* Anim = GetAnimInst();
	//Anim->BindStateDelegate(UCreatureAnimInstance::AniDelegateType::Enter, TEXT("UniqueSkillExit"), std::bind(&RogSkillState::ExcuteBlink, this));
}

void RogSkillState::ExcuteSkill(const FVector& _AttackLocation)
{
	//도적은 나중에나 만들자

	/*ANetworkPlayerController* PlayerCtrl = GetOwner<ANetworkPlayerController>();
	check(PlayerCtrl);
	ANetworkPlayer* Player = PlayerCtrl->GetPlayer();
	float HalfHeight = Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
	MoveDestination = _AttackLocation + (FVector::UpVector * HalfHeight);
	FVector PlayerLocation = Player->GetActorLocation();
	FVector Dir = (MoveDestination - PlayerLocation);
	Dir.Z = 0.f;
	Dir.Normalize();
	FVector ParticleSpawnPos = (PlayerLocation + Dir);
	
	Protocol::C_ATTACK AttackPkt;
	Protocol::PosInfo* LaunchPos = AttackPkt.mutable_target_pos();
	LaunchPos->set_x(ParticleSpawnPos.X);
	LaunchPos->set_y(ParticleSpawnPos.Y);
	AttackPkt.set_attack_type(Protocol::Attack_TYPE_BLINK);
	AttackPkt.set_yaw(Dir.Rotation().Yaw);
	SEND_PACKET(AttackPkt);*/
}

void RogSkillState::ExcuteBlink()
{
	/*ANetworkPlayerController* PlayerCtrl = GetOwner<ANetworkPlayerController>();
	check(PlayerCtrl);
	ANetworkPlayer* Player = PlayerCtrl->GetPlayer();

	Move(Player);
	SendMovePacket(Player);*/
}

void RogSkillState::Move(ANetworkPlayer* _Player)
{
	FVector CurLocation = _Player->GetActorLocation();
	FVector MoveDir = (MoveDestination - CurLocation);
	MoveDir.Normalize();

	float Yaw = MoveDir.Rotation().Yaw;
	_Player->SetActorRotation(FRotator(0.f, Yaw, 0.f));
	_Player->SetActorLocation(MoveDestination);

	Protocol::PosInfo& CurPos = _Player->GetProtocolPos();
	CurPos.set_x(MoveDestination.X);
	CurPos.set_y(MoveDestination.Y);
	CurPos.set_z(MoveDestination.Z);
}

void RogSkillState::SendMovePacket(ANetworkPlayer* _Player)
{
	/*const Protocol::ObjectInfo& ObjInfo = _Player->GetProtocolInfo();
	const Protocol::PosInfo& PosInfo = _Player->GetProtocolPos();

	Protocol::C_MOVE Pkt;
	Pkt.set_object_id(ObjInfo.object_id());
	Pkt.mutable_current_pos()->CopyFrom(PosInfo);
	Pkt.set_state(Protocol::CREATURE_STATE_IDLE);
	Pkt.set_fix_pos(true);
	SEND_PACKET(Pkt);*/
}

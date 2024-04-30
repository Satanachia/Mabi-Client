#pragma once
#include "FSM/BaseState.h"
#include "Protocol.pb.h"

class ANetworkOthers;
class UPlayerAnimInstance;

class AttackState : public BaseState
{
protected:
	virtual void Start() override;
	virtual void EnterState() override;

private:
	void LookTarget();
	void SendAnimationPacket();
	void SendAttackPacket();

	virtual void OnInteractionCallBack(PlayerInputType _Type) override;
	void AnimNotify_Attack();
	void AnimNotify_Finish();

private:
	enum { MIN_COMBO = 0, MAX_COMBO = 3 };

	ANetworkOthers* Target = nullptr;
	int32 ReserveCombo = 0;
};

#pragma once

#include "FSM/BaseState.h"

class MoveState : public BaseState
{
public:
	static bool IsMovableState(PlayerStateType _State);

protected:
	virtual void Start() override;
	virtual void Update(float _DeltaTime) override;
	virtual void ExitState() override;
	
	virtual void OnInteractionCallBack(PlayerInputType _Type) override;

private:
	void SendMovePacket();

private:
	float MoveSpeed = 0.f;
	float MovePacketTimer = 0.f;

	//일단 임시
	const float AttackRangeDistance = 100.f;
};

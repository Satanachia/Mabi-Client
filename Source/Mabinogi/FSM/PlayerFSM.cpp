#include "FSM/PlayerFSM.h"
#include "FSM/BaseState.h"

PlayerFSM::PlayerFSM()
{
}

PlayerFSM::~PlayerFSM()
{
	for (int32 i = 0; i < PLAYER_STATE_COUNT; ++i)
	{
		if (nullptr == AllStates[i])
			continue;

		delete AllStates[i];
		AllStates[i] = nullptr;
	}
}

void PlayerFSM::ChangeState(PlayerStateType _Key, bool _IsForce /*= false*/)
{
	check(PLAYER_STATE_IDLE <= _Key && _Key < PLAYER_STATE_COUNT);
	BaseState* NextState = AllStates[_Key];
	check(NextState);
	
	//������ ���� ������ ���
	if (NextState == CurrentState && false == _IsForce)
		return;

	//�ܺο��� ���¸� �������� ���ϰ� �� ���
	if (nullptr != CurrentState && false == CurrentState->IsChangable(_Key))
		return;
	
	if (nullptr != CurrentState)
	{
		CurrentState->ExitState();
	}
	
	CurrentState = NextState;
	NextState->EnterState();
}

void PlayerFSM::Update(float _DeltaTime)
{
	if (nullptr == CurrentState)
		return;
	
	CurrentState->Update(_DeltaTime);
}

PlayerStateType PlayerFSM::GetCurState()
{
	return CurrentState->Key;
}

void PlayerFSM::OnInteractionCallBack(PlayerInputType _Type)
{
	CurrentState->OnInteractionCallBack(_Type);
}

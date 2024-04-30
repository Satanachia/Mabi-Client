#pragma once
#include <unordered_map>
#include "Define.h"


class BaseState;
class ANetworkPlayerController;
using int32 = int;

class PlayerFSM
{
public:
	PlayerFSM();
	~PlayerFSM();

public:
	template <typename T>
	void CreateState(PlayerStateType _Key, ANetworkPlayerController* _Ctrl)
	{
		BaseState* NewState = new T;
		check(nullptr == AllStates[_Key]);
		AllStates[_Key] = NewState;

		NewState->PlayerCtrl = _Ctrl;
		NewState->Fsm = this;
		NewState->Key = _Key;

		NewState->Start();
	}

	void ChangeState(PlayerStateType _Key, bool _IsForce = false);
	void Update(float _DeltaTime);

	PlayerStateType GetCurState();

	void OnInteractionCallBack(PlayerInputType _Type);

private:
	BaseState* AllStates[PLAYER_STATE_COUNT] = { nullptr, };
	BaseState* CurrentState = nullptr;
};

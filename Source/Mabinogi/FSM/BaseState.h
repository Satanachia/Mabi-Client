#pragma once
#include "Define.h"

class ANetworkPlayerController;
class ANetworkPlayer;
class PlayerFSM;
class UPlayerAnimInstance;
using int32 = int;


class BaseState
{
	friend class PlayerFSM;
	
public:
	BaseState(){}
	virtual ~BaseState(){}

protected:
	virtual void Start();
	virtual void EnterState();
	virtual void Update(float _DeltaTime);
	virtual void ExitState();

	virtual void OnInteractionCallBack(PlayerInputType _Type){}
	
protected:
	ANetworkPlayerController* GetPlayerController() { return PlayerCtrl; }
	ANetworkPlayer* GetPlayer();
	UPlayerAnimInstance* GetPlayerAnimInst();
	
	void ChangeState(PlayerStateType _Key, bool _IsForce = false);
	
	PlayerStateType GetCurrentState();
	PlayerStateType GetThisState() { return Key; }
	bool IsThisWorking();
	
	//인자로 주어진 상태를 제외하고 외부에서 FSM을 변경하는 것을 막음
	void BlockOutsideChange(const std::vector<PlayerStateType>& _ChangableStates = std::vector<PlayerStateType>());

private:
	bool IsChangable(PlayerStateType _State);

private:
	ANetworkPlayerController* PlayerCtrl = nullptr;
	PlayerFSM* Fsm = nullptr;
	PlayerStateType Key = PLAYER_STATE_COUNT;

	int32 OutsideChangeBit = -1;
};

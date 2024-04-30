#pragma once
#include "FSM/BaseState.h"
#include "Define.h"

class ANetworkPlayerController;
class ANetworkPlayer;
class PlayerFSM;
using int32 = int;


class IdleState : public BaseState
{
	friend class PlayerFSM;
	
public:
	IdleState(){}
	virtual ~IdleState(){}

protected:
	virtual void OnInteractionCallBack(PlayerInputType _Type) override;

private:

};

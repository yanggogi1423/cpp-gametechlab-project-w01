#pragma once
#include "IState.h"
class EndingState : public IState
{
	EndingState() = default;
	virtual ~EndingState() = default;
	void OnEnter(UManager* manager) override;
	IState* Update(URenderer* renderer) override;
	void OnExit() override;
};


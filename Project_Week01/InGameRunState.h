#pragma once
#include "IState.h"
class InGameRunState : public IState
{
	InGameRunState() = default;
	virtual ~InGameRunState() = default;
	void OnEnter(UManager* manager) override;
	IState* Update(URenderer* renderer) override;
	void OnExit() override;
};


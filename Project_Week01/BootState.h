#pragma once
#include "IState.h"
class BootState : public IState
{
	BootState() = default;
	virtual ~BootState() = default;
	void OnEnter() override;
	IState* Update(URenderer* renderer) override;
	void OnExit() override;
};


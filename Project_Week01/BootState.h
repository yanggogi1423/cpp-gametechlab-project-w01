#pragma once
#include "IState.h"
class BootState : public IState
{
	BootState() = default;
	virtual ~BootState() = default;
	void OnEnter(UManager * manager) override;
	IState* Update(URenderer* renderer) override;
	void OnExit() override;
};


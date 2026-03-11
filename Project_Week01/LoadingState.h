#pragma once
#include "IState.h"

class LoadingState : public IState
{
	LoadingState() = default;
	virtual ~LoadingState() = default;
	void OnEnter() override;
	IState* Update(URenderer* renderer) override;
	void OnExit() override;
};


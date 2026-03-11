#pragma once
#include "IState.h"

class LoadingState : public IState
{
	LoadingState() = default;
	virtual ~LoadingState() = default;
	void OnEnter(UManager* manager) override;
	IState* Update(URenderer* renderer) override;
	void OnExit() override;
};


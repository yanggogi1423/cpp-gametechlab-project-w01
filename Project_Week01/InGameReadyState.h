#pragma once
#include "IState.h"

class InGameReadyState : public IState
{
	InGameReadyState() = default;
	virtual ~InGameReadyState() = default;
	void OnEnter(UManager* manager) override;
	IState* Update(URenderer* renderer) override;
	void OnExit() override;
};


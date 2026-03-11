#pragma once
#include "IState.h"

class InGameReadyState : public IState
{
public:
	InGameReadyState() = default;
	virtual ~InGameReadyState() = default;
	void OnEnter() override;
	IState* Update(URenderer* renderer) override;
	void OnExit() override;
};


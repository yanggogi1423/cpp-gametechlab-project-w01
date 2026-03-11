#pragma once
#include "IState.h"

class StageSelectionState : public IState
{
	StageSelectionState() = default;
	virtual ~StageSelectionState() = default;
	void OnEnter(UManager * manager) override;
	IState* Update(URenderer* renderer) override;
	void OnExit() override;
};


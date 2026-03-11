#pragma once
#include "IState.h"

class StageSelectionState : public IState
{
public:
	StageSelectionState() = default;
	virtual ~StageSelectionState() = default;
	void OnEnter() override;
	IState* Update(URenderer* renderer) override;
	void OnExit() override;
};


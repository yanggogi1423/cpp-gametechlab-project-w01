#pragma once
#include "IState.h"

class StageSelectionState : public IState
{
public:
	StageSelectionState() = default;
	virtual ~StageSelectionState() = default;

	void OnEnter(class UManager* manager) override;
	IState* Update(float deltaTime, class UManager* manager) override;
	void Render(class URenderer* renderer, class UManager* manager) override;
	void OnExit(class UManager* manager) override;
};

#pragma once
#include "IState.h"

class MainState : public IState
{
public:
	MainState() = default;
	~MainState() = default;
	void OnEnter() override;
	IState* Update(URenderer* renderer) override;
	void OnExit() override;


	//예시용
	IState* GoToExampleState();
};


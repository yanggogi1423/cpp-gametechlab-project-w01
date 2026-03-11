#pragma once
#include "IState.h"

class ExampleState : public IState
{
public:
	ExampleState() = default;
	virtual ~ExampleState() = default;
	void OnEnter(UManager* manager) override;
	IState* Update(URenderer* renderer) override;
	void OnExit() override;



	IState* BackToMainState();
};


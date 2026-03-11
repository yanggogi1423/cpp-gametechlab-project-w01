#pragma once
#include "IState.h"

class MainState : public IState
{
public:
	MainState() = default;
	virtual ~MainState() = default;
	
	void OnEnter(UManager* manager) override;
	IState* Update(float deltaTime, UManager* manager) override;
	void Render(URenderer* renderer, UManager* manager) override;
	void OnExit(UManager* manager) override;
};


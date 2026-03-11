#pragma once
#include "IState.h"
#include "MainState.h"

class StateMachine
{
private:
	IState* currentState = nullptr;

public: 
	StateMachine();
	~StateMachine();
	void Update(URenderer* renderer);

};


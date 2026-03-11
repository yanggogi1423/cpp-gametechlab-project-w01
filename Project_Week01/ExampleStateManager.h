#pragma once
#include "StateMachine.h"

class ExampleStateManager
{
private:
	StateMachine* stateMachine;

public:
	ExampleStateManager();
	~ExampleStateManager();
	void Update(URenderer* renderer);
};


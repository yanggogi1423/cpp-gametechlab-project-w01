#pragma once
#include "StateMachine.h"

class ExampleStateManager
{
private:
	StateMachine* stateMachine;
	UManager* Manager;

public:
	ExampleStateManager(UManager * manager);
	~ExampleStateManager();
	void Update(URenderer* renderer);
};


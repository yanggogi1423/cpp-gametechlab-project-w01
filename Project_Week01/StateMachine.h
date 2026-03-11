#pragma once
#include "IState.h"
#include "MainState.h"

class UManager;

class StateMachine
{
private:
	IState* currentState = nullptr;
	UManager* Manager;
public: 
	StateMachine(UManager * manager);
	~StateMachine();
	void Update(URenderer* renderer);

};


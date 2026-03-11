#include "StateMachine.h"


StateMachine::StateMachine()
{
	currentState = new MainState();
	currentState->OnEnter();
}

StateMachine::~StateMachine()
{
	if (currentState)
	{
		currentState->OnExit();
		delete currentState;
		currentState = nullptr;
	}
}

void StateMachine::Update(URenderer* renderer)
{
	if (currentState)
	{
		IState* nextState = currentState->Update(renderer);
		if (nextState != currentState)
		{
			currentState->OnExit();
			delete currentState;
			currentState = nextState;
			currentState->OnEnter();
		}
	}
}
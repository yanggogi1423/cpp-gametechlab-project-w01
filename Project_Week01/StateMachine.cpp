#include "StateMachine.h"
#include "UManager.h"

StateMachine::StateMachine() : currentState(nullptr) {}

StateMachine::~StateMachine()
{
	if (currentState)
	{
		// 종료 시 메모리 누수 방지
		delete currentState;
		currentState = nullptr;
	}
}

void StateMachine::Initialize(IState* initialState, UManager* manager)
{
	currentState = initialState;
	if (currentState) currentState->OnEnter(manager);
}

void StateMachine::Update(float deltaTime, UManager* manager)
{
	if (currentState)
	{
		IState* next = currentState->Update(deltaTime, manager);

		if (next != currentState)
		{
			currentState->OnExit(manager);
			delete currentState;
			currentState = next;
			currentState->OnEnter(manager);
		}
	}
}

void StateMachine::Render(URenderer* renderer, UManager* manager)
{
	if (currentState)
	{
		currentState->Render(renderer, manager);
	}
}
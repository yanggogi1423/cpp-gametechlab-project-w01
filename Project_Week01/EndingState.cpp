#include "EndingState.h"

void EndingState::OnEnter()
{

}

IState* EndingState::Update(URenderer* renderer)
{
	nextState = this;

	if (uiManager)
		uiManager->Render();
	return nextState;

}

void EndingState::OnExit()
{
	delete uiManager;
}
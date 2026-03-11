#include "InGameRunState.h"

void InGameRunState::OnEnter()
{

}

IState* InGameRunState::Update(URenderer* renderer)
{
	nextState = this;

	if (uiManager)
		uiManager->Render();
	return nextState;

}

void InGameRunState::OnExit()
{
	delete uiManager;
}
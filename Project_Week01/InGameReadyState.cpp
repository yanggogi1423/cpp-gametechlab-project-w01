#include "InGameReadyState.h"

void InGameReadyState::OnEnter()
{

}

IState* InGameReadyState::Update(URenderer* renderer)
{
	nextState = this;

	if (uiManager)
		uiManager->Render();
	return nextState;

}

void InGameReadyState::OnExit()
{
	delete uiManager;
}
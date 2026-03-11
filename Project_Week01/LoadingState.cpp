#include "LoadingState.h"

void LoadingState::OnEnter()
{

}

IState* LoadingState::Update(URenderer* renderer)
{
	nextState = this;

	if (uiManager)
		uiManager->Render();
	return nextState;

}

void LoadingState::OnExit()
{
	delete uiManager;
}
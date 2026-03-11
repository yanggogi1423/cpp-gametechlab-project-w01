#include "InGameRunState.h"

void InGameRunState::OnEnter(UManager * manager)
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
#include "InGameReadyState.h"

void InGameReadyState::OnEnter(UManager * manager)
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
#include "BootState.h"

void BootState::OnEnter()
{

}

IState* BootState::Update(URenderer* renderer)
{
	nextState = this;

	if (uiManager)
		uiManager->Render();
	return nextState;

}

void BootState::OnExit()
{
	delete uiManager;
}
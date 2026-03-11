#include "BootState.h"

void BootState::OnEnter(UManager * manager)
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
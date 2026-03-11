#include "StageSelectionState.h"

void StageSelectionState::OnEnter(UManager * manager)
{

}

IState* StageSelectionState::Update(URenderer* renderer)
{
	nextState = this;

	if (uiManager)
		uiManager->Render();
	return nextState;

}

void StageSelectionState::OnExit()
{
	delete uiManager;
}
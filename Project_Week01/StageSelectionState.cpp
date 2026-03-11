#include "StageSelectionState.h"

void StageSelectionState::OnEnter()
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
#pragma once
#include "UIManager.h"
#include "URenderer.h"

class IState
{
protected:
	UIManager* uiManager = nullptr;
	IState* nextState = nullptr;
public:
	virtual ~IState() = default;
	virtual void OnEnter() = 0;
	virtual IState* Update(URenderer* renderer) = 0;
	virtual void OnExit() = 0;
};
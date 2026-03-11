#pragma once
#include "IState.h"

class MainState : public IState
{
public:
	MainState() = default;
	~MainState() = default;
	void OnEnter(UManager * manager) override;
	IState* Update(URenderer* renderer) override;
	void OnExit() override;

	//	LOGO Animation
	const float TopOffsetY = -25.f;
	const float BottomOffsetY = 25.f;
	float OffsetY = 0.f;
	float DY = -0.5f;

	UIFrame* LogoFrame;

	//	Contributors
	UIFrame* InfoFrame;
	//bool bInfoVisible = false;

	//예시용
	IState* GoToExampleState();
};


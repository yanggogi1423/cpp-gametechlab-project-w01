#include "MainState.h"
#include "ExampleState.h"

void MainState::OnEnter()
{
	uiManager = new UIManager();

	UIFrame& frame1 = uiManager->CreateFrame("MainState")
		.Position(ImVec2(10, 10))
		.Size(ImVec2(200, 300));

	frame1.AddButton("GoToExample", ImVec2(20, 20), ImVec2(20, 20), [&] {
		nextState = this->GoToExampleState();
		});



	//여기서부터 init코드 작성
}

IState* MainState::Update(URenderer* renderer)
{
	nextState = this;


	//조건 분기
	//다음 State를 return 하는 함수를 작성. uiFrame 버튼에 람다로 등록.


	if(uiManager)
		uiManager->Render();
	return nextState;
}

void MainState::OnExit()
{
	delete uiManager;
}

IState* MainState::GoToExampleState()
{
	return new ExampleState();
}
#include "MainState.h"
#include "ExampleState.h"

#include "UManager.h"

void MainState::OnEnter(UManager * manager)
{
	uiManager = new UIManager();

	UIFrame& bgFrame = uiManager->CreateFrame("MainState")
		.Position(ImVec2(0, 0))
		.Size(ImVec2(1400, 1050))
		.NoTitleBar(true)
		.BackgroundColor(ImVec4(0, 0, 0, 0));

	//	Background
	const int seg = 3;
	const int dx = WindowWidth / seg;
	
	for (int y = 0; y < seg; y++)
	{
		for (int x = 0; x < seg; x++)
		{
			bgFrame.AddImage(manager->GetResourceManager()->GetTexture("Background"),
				ImVec2(dx * x, dx * y),
				ImVec2(dx, dx));
		}
	}
	
	UIFrame & logoFrame = uiManager->CreateFrame("MainState")
		.Position(ImVec2(0, 0))
		.Size(ImVec2(1400, 1050))
		.NoTitleBar(true)
		.BackgroundColor(ImVec4(0, 0, 0, 0)
		);

	//	Logo
	logoFrame.AddText("SWING BY ME",
		ImVec2(WindowWidth / 2, WindowHeight / 3),
		manager->GetResourceManager()->FontLogo
		);
	
	//	Button Set
	logoFrame.AddSpriteButton("Game Start",
		manager->GetResourceManager()->SRVButtonSprite,
		ImVec2(Window)
		);


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
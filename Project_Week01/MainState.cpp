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


	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	//	Background
	const int tileSize = 350;
	const int segX = WindowWidth / tileSize;   // 20
	const int segY = WindowHeight / tileSize;  // 15

	for (int y = 0; y < segY; y++)
	{
		for (int x = 0; x < segX; x++)
		{
			bgFrame.AddImage(
				manager->GetResourceManager()->GetTexture("Background"),
				ImVec2((float)(tileSize * x), (float)(tileSize * y)),
				ImVec2((float)tileSize, (float)tileSize)
			);
		}
	}

	bgFrame.AddText("v.1.0.0 by GameTechLab Team 3", ImVec2(133, WindowHeight - 17), manager->GetResourceManager()->FontInfoLight);

	ImGui::PopStyleVar();
	
	UIFrame & logoFrame = uiManager->CreateFrame("MainState")
		.Position(ImVec2(0, 0))
		.Size(ImVec2(1400, 1050))
		.NoTitleBar(true)
		.BackgroundColor(ImVec4(0, 0, 0, 0)
		);

	//	For Animation
	LogoFrame = &logoFrame;

	//	Logo
	logoFrame.AddText("SPACE.EXE",
		ImVec2(WindowWidth / 2, WindowHeight / 3),
		manager->GetResourceManager()->FontLogo
		);
	
	UIFrame& btnFrame = uiManager->CreateFrame("MainState")
		.Position(ImVec2(0, 0))
		.Size(ImVec2(1400, 1050))
		.NoTitleBar(true)
		.BackgroundColor(ImVec4(0, 0, 0, 0)
		);

	//	Button Set
	btnFrame.AddSpriteButton9("Game Start",
		manager->GetResourceManager()->SRVButtonSprite,
		ImVec2(WindowWidth / 2, WindowHeight / 2 + 80),
		ImVec2(400, 90),
		2,
		6.0f,
		nullptr
		);

	btnFrame.AddText("Game Start",
		ImVec2(WindowWidth / 2 + 10, WindowHeight / 2 + 80),
		manager->GetResourceManager()->FontDefault
	);

	btnFrame.AddSpriteButton9("Exit",
		manager->GetResourceManager()->SRVButtonSprite,
		ImVec2(WindowWidth / 2, WindowHeight / 2 + 200),
		ImVec2(400, 90),
		2,
		6.0f,
		nullptr
	);

	btnFrame.AddText("Exit",
		ImVec2(WindowWidth / 2 + 10, WindowHeight / 2 + 200),
		manager->GetResourceManager()->FontDefault
	);

	btnFrame.AddSpriteButton("Info",
		manager->GetResourceManager()->SRVButtonSprite,
		ImVec2(WindowWidth - 100, WindowHeight - 100),
		ImVec2(80,80),
		27,
		//	Toggle
		[&]() {
			InfoFrame->bVisibility = !InfoFrame->bVisibility;
		}
		);

	UIFrame& infoFrame = uiManager->CreateFrame("MainState")
		.Position(ImVec2(0, 0))
		.Size(ImVec2(1400, 1050))
		.NoTitleBar(true)
		.BackgroundColor(ImVec4(0, 0, 0, 0)
		);

	InfoFrame = &infoFrame;
	infoFrame.bVisibility = false;

	infoFrame.AddImage(manager->GetResourceManager()->SRVLeaderBoardPanel,
		ImVec2(WindowWidth / 2 - 470, WindowHeight / 2 - 375),
		ImVec2(940, 750)
	);

	infoFrame.AddText("Contributors",
		ImVec2(WindowWidth / 2, WindowHeight / 2 - 290),
		manager->GetResourceManager()->FontInfoBold
	);

	infoFrame.AddText("Kim KiHoon (PM)",
		ImVec2(WindowWidth / 2, WindowHeight / 2 - 190),
		manager->GetResourceManager()->FontInfoRegular);

	infoFrame.AddText("PARK SangHyuk",
		ImVec2(WindowWidth / 2, WindowHeight / 2 - 110),
		manager->GetResourceManager()->FontInfoRegular);

	infoFrame.AddText("YANG HyunSeok",
		ImVec2(WindowWidth / 2, WindowHeight / 2 - 30),
		manager->GetResourceManager()->FontInfoRegular);

	infoFrame.AddText("HAN YounSeo",
		ImVec2(WindowWidth / 2, WindowHeight / 2 + 50),
		manager->GetResourceManager()->FontInfoRegular);

	//여기서부터 init코드 작성
}

IState* MainState::Update(URenderer* renderer)
{
	nextState = this;


	//조건 분기
	//다음 State를 return 하는 함수를 작성. uiFrame 버튼에 람다로 등록.

	//	Logo Animation
	if (LogoFrame)
	{
		if (OffsetY <= TopOffsetY || OffsetY >= BottomOffsetY)
		{
			DY *= -1;
		}

		OffsetY += DY;
		LogoFrame->SetPosition(ImVec2(0, OffsetY));
	}


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
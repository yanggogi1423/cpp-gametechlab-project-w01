#include "InGameReadyState.h"
#include "UManager.h" // 매니저의 자원 접근을 위해 포함

#include "MainState.h"
#include "InGameRunState.h"

#pragma region __DEBUG_CONSOLE__
#include <iostream>
#pragma endregion

void InGameReadyState::OnEnter(UManager* manager)
{
	/* Placement Manager */

	PlanetPlacementManager = new UPlanetPlacementManager(manager);

	// 1. 기존 객체 제거 (ClearGameObjects는 public으로 변경 필요)
	manager->ClearGameObjects();


	// 2. 스테이지 정보 추출 (필요한 Getter 추가가 필수적입니다)
	// CurStage와 StageInfoList는 현재 private이므로 Getter를 통해 가져와야 합니다.
	int StageIdx = (int)manager->GetCurStage() - 1;
	const auto& stageInfoList = manager->GetStageInfoList();


	if (StageIdx < 0 || StageIdx >= (int)stageInfoList.size()) return; 

	const FStageInfo& stageInfo = stageInfoList[StageIdx];
	manager->SetRemainTimer(stageInfo.MaxTime); // 타이머 설정

	// 스테이지 번호에 따른 BGM 전환
	EStage curStage = manager->GetCurStage();

	switch (curStage)
	{
	case EStage::ES_Stage1:
		manager->PlayBGM(EBGM::EBGM_Level1);
		break;
	case EStage::ES_Stage2:
		manager->PlayBGM(EBGM::EBGM_Level2);
		break;
	case EStage::ES_Stage3:
		manager->PlayBGM(EBGM::EBGM_Level3);
		break;
	default:
		manager->PlayBGM(EBGM::EBGM_TitleScreen);
		break;
	}

	// 3. 장애물(행성) 배치 로직
	for (auto& obstacle : stageInfo.ObstacleList)
	{
		USphere planet;
		planet.SetLocation(obstacle.second);
		planet.SetMass(10.0f);
		planet.SetScale(0.1f);

		// CreateNewPlanetWorld를 사용하여 행성 추가
		manager->CreateNewPlanetWorld(planet);
	}

	// 4. 플레이어 배치 로직
	Probe* player = manager->GetProbe(); // 기존 public 함수 사용
	if (player == nullptr) {
		player = new Probe();
		manager->SetPlayer(player); // Player 포인터 설정을 위한 Setter 추가 필요
	}
	// 5. Goal 생성 & 배치
	Goal goal = stageInfo.goal;
	// 플레이어의 위치는 레벨에 따라 세팅 
	switch (StageIdx)
	{
	case 0:
		player->SetLocation({ 1.0f, -1.0f, 0.0f });
		player->SetVelocity(FVector(0.0f, 1.0f, 0.0f) * 0.0f);
		goal.SetLocation({ 0.0f , 0.0f , 0.0f }); 
		break;
	case 1:
		player->SetLocation({ 0.5f , 1.0f , 0.0f });
		player->SetVelocity({ 0.0f , 1.0f , 0.0f });
		goal.SetLocation({ -1.0f ,-1.0f , 0.0f });
		break;
	case 2:
		player->SetLocation({ -1.0f , 0.0f , 0.0f });
		player->SetVelocity({ 0.0f , 1.0f , 0.0f });
		goal.SetLocation({ 0.5f , 0.0f , 0.0f });
		break;
	default:
		player->SetLocation({ 0.0f, -1.0f, 0.0f });
		player->SetVelocity({ 0.0f , 1.0f , 0.0f });
		goal.SetLocation({ 0.0f , 1.0f , 0.0f });
		break;
	}
	player->SetScale(0.1f);


	// 5. UI 초기화 (기존 로직 유지)
	uiManager = new UIManager();

	UIFrame& bgFrame = uiManager->CreateFrame("MainState")
		.Position(ImVec2(0, 0))
		.Size(ImVec2(1400, 1050))
		.NoTitleBar(true)
		.BackgroundColor(ImVec4(0, 0, 0, 0));


	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
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

	UIFrame& HUDFrame = uiManager->CreateFrame("Ready Phase")
		.Position(ImVec2(0, 0))
		.Size(ImVec2(WindowWidth, WindowHeight))
		.NoTitleBar(true)
		.BackgroundColor(ImVec4(0, 0, 0, 0));

	HUDFrame.AddImage(manager->GetResourceManager()->SRVInGamePanel,
		ImVec2(WindowWidth * 3 / 4.f, 0),
		ImVec2(WindowWidth / 4, WindowHeight)
	);

	HUDFrame.AddImageButton("Planet 1",
		manager->GetResourceManager()->GetTexture(static_cast<ImageName>(USphere::count)),
		ImVec2(WindowWidth * 3 / 4.f + 150, 220),
		ImVec2(100, 100),
		[&]() {
			USphere* newPlanet = new USphere();
			PlanetPlacementManager->SetSelectedPlanet(newPlanet);
		}
	); 


	HUDFrame.AddImageButton("Start",
		manager->GetResourceManager()->SRVLeaderBoardPanel,
		ImVec2(WindowWidth * 3 / 4.f + 150, 700),
		ImVec2(100, 50),
		[&]() {
			bGoToStart = true;
		}
	);
	HUDFrame.AddText("Start",
		ImVec2(WindowWidth * 3 / 4.f + 150, 700),
		manager->GetResourceManager()->FontInfoLight);

	HUDFrame.AddImageButton("Retry",
		manager->GetResourceManager()->SRVLeaderBoardPanel,
		ImVec2(WindowWidth * 3 / 4.f + 150, 800),
		ImVec2(100, 50),
		[&]() {
			bGoToRetry = true;
		}
	);
	HUDFrame.AddText("Retry",
		ImVec2(WindowWidth * 3 / 4.f + 150, 800),
		manager->GetResourceManager()->FontInfoLight);


	HUDFrame.AddImageButton("Home",
		manager->GetResourceManager()->SRVLeaderBoardPanel,
		ImVec2(WindowWidth * 3 / 4.f + 150, 900),
		ImVec2(100, 50),
		[&]() {
			bGoToMain = true;
		}
	);
	HUDFrame.AddText("Home",
		ImVec2(WindowWidth * 3 / 4.f + 150, 900),
		manager->GetResourceManager()->FontInfoLight);

}

IState* InGameReadyState::Update(float deltaTime, UManager* manager)
{
	nextState = this;
	// 준비 단계에서는 마우스 클릭으로 행성을 배치하는 로직 등을 추가할 수 있습니다.

	if(bGoToMain)
	{
		nextState = new MainState();
	}
	else if (bGoToRetry)
	{
		nextState = new InGameReadyState();
	}
	else if (bGoToStart)
	{
		nextState = new InGameRunState();
	}

	PlanetPlacementManager->Update(deltaTime);

	return nextState;
}


// texture 렌더링
void InGameReadyState::Render(URenderer* renderer, UManager* manager)
{

	MeshResource* goal = manager->getGoalResource();
	if (goal != nullptr) {
		renderer->textureRenderPrimitive(goal->VB, goal->IB, goal->IndexCount, manager->GetResourceManager()->GetTexture(ImageName::GOAL));
	}
	if (uiManager) 
	{
		uiManager->Render();
		//std::cout << "UI Manager Rendered!" << std::endl;
	}
	//else std::cout << "UI Manager is null!" << std::endl;

	// 1. 플레이어 렌더링
	Probe* pPlayer = manager->GetProbe();

	//if (pPlayer == nullptr) {
	//	std::cout << "Player is null!" << std::endl;
	//}



	if (pPlayer) {
		renderer->UpdateConstant(pPlayer->GetTransformMatrix());
		MeshResource* res = manager->getProbeResource();
		renderer->textureRenderPrimitive(res->VB, res->IB, res->IndexCount, manager->GetResourceManager()->GetTexture(ImageName::ROCKET));
	}

	// 2. 행성들 렌더링
	MeshResource* sphereRes = manager->getSphereResource();
	for (auto& planet : manager->GetPlanetList()) {
		renderer->UpdateConstant(planet.GetTransformMatrix());
		renderer->textureRenderPrimitive(sphereRes->VB, sphereRes->IB, sphereRes->IndexCount, manager->GetResourceManager()->GetTexture(planet.getImageName()));
	}

	// 클릭하면 그림
	if (PlanetPlacementManager->IsPlacable())
	{
		USphere* selectedPlanet = PlanetPlacementManager->GetSelectedPlanet();
		if (selectedPlanet)
		{
			renderer->UpdateConstant(selectedPlanet->GetTransformMatrix());
			renderer->textureRenderPrimitive(sphereRes->VB, sphereRes->IB, sphereRes->IndexCount, manager->GetResourceManager()->GetTexture(selectedPlanet->getImageName()));
			
		}
	}


}

void InGameReadyState::OnExit(UManager* manager)
{
	if (uiManager) {
		delete uiManager;
		uiManager = nullptr;
	}
}
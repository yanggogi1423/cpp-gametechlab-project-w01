#include "InGameRunState.h"
#include "UManager.h" // 매니저의 자원 접근을 위해 포함

#include "MainState.h"
#include "InGameReadyState.h"

#include<iostream>

void InGameRunState::OnEnter(UManager* manager)
{
	// 1. [이사 완료] UManager::InGameRunInit 로직
	// 이제 이 상태에 진입한 것 자체가 'InGameRun'이 시작되었음을 의미합니다.

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

	HUDFrame.AddText("Simulating...",
		ImVec2(WindowWidth * 3 / 4.f + 150, 300),
		manager->GetResourceManager()->FontInfoLight);

	//std::string timerText = "Remain Time : " + std::to_string(manager->GetRemainTimer());

	//HUDFrame.AddText("Remain Time : " + timerText,
	//	ImVec2(WindowWidth * 3 / 4.f + 150, 500),
	//	manager->GetResourceManager()->FontInfoLight);

	HUDFrame.AddSelectableText("Timer", "Remain Time : " + std::to_string(manager->GetRemainTimer()),
		ImVec2(WindowWidth * 3 / 4.f + 150, 500),
		manager->GetResourceManager()->FontInfoLight);

	//HUDFrame.AddImageButton("Planet 1",
	//	manager->GetResourceManager()->GetTexture(ImageName::SATURN),
	//	ImVec2(WindowWidth * 3 / 4.f + 150, 220),
	//	ImVec2(100, 100),
	//	[&]() {
	//		USphere* newPlanet = new USphere();
	//		PlanetPlacementManager->SetSelectedPlanet(newPlanet);
	//	}
	//);


	//HUDFrame.AddImageButton("Start",
	//	manager->GetResourceManager()->SRVLeaderBoardPanel,
	//	ImVec2(WindowWidth * 3 / 4.f + 150, 700),
	//	ImVec2(100, 50),
	//	[&]() {
	//		bGoToStart = true;
	//	}
	//);
	//HUDFrame.AddText("Start",
	//	ImVec2(WindowWidth * 3 / 4.f + 150, 700),
	//	manager->GetResourceManager()->FontInfoLight);

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

	hudFrame = &HUDFrame;
}

IState* InGameRunState::Update(float deltaTime, UManager* manager)
{
	nextState = this;

	float remainTime = manager->GetRemainTimer(); // Getter 필요

	remainTime = remainTime - deltaTime;
	remainTime = max(0.f, remainTime);
	manager->SetRemainTimer(remainTime); // Setter 필요

	auto textInfo = hudFrame->GetSelectableText("Timer");
	auto remainTimestr = std::to_string(manager->GetRemainTimer()).substr(0, 4); //문자열에서 2자리수.소수점 1자리 => 4자리
	textInfo->text = "Remain Time : " + remainTimestr;
	std::cout << textInfo->text << std::endl;

	if (remainTime <= 0.f)
	{
		manager->SetSuccess(false);
		return new EndingState();
	}

	auto planetList = manager->GetPlanetList();
	auto player = manager->GetProbe();
	auto stageInfoList = manager->GetStageInfoList();
	auto curStage = manager->GetCurStage();

	for (auto p : planetList)
	{
		// 1. 방향 벡터 및 거리 계산
		FVector direction = p.GetLocation() - player->GetLocation();
		float dist = direction.Size();
		if (dist < 1e-4f) continue; // 0으로 나누기 방지

		// 2. 가속도 크기 및 방향 벡터(단위 벡터) 계산
		FVector unitDir = direction / dist;
		float accMag = (GravititationalConstant * p.GetMass()) / (float)pow(dist, 2);
		FVector accVec = unitDir * accMag;

		// 3. 속도 업데이트
		player->SetVelocity(player->GetVelocity() + accVec * deltaTime);
	}

	FVector pLoc = player->GetLocation();

	//	Collision between Probe and Planets
	for (const auto& p : planetList)
	{
		float dist = (p.GetLocation() - pLoc).Size();
		if (dist < p.GetScale() + player->GetScale())
		{
			player->SetColliding(true);
			manager->SetSuccess(false);
			return new EndingState();
		}
	}

	// Collision between Probe and Exit Location
	float goalDist = (stageInfoList[(int)curStage - 1].goal.GetLocation() - pLoc).Size();
	if (goalDist < 0.15f)
	{
		manager->SetSuccess(true);
		return new EndingState();
	}


	if (bGoToMain)
	{
		nextState = new MainState();
	}
	else if (bGoToRetry)
	{
		nextState = new InGameReadyState();
	}


	return nextState;
}

void InGameRunState::Render(URenderer* renderer, UManager* manager)
{
	// 4. [이사 완료] main.cpp에 있던 3D 객체 렌더링 루프

	// (3) UI 렌더링 (필요 시)
	if (uiManager) uiManager->Render();

	// (1) 플레이어(Probe) 렌더링
	Probe* pPlayer = manager->GetProbe();
	if (pPlayer != nullptr)
	{
		renderer->UpdateConstant(pPlayer->GetTransformMatrix());
		MeshResource* probeRes = manager->getProbeResource();
		if (probeRes->VB != nullptr)
		{
			//renderer->indexRenderPrimitive(probeRes->VB, probeRes->IB, probeRes->IndexCount);
			renderer->textureRenderPrimitive(probeRes->VB, probeRes->IB, probeRes->IndexCount, manager->GetResourceManager()->GetTexture(ImageName::ROCKET));
		}
	}

	MeshResource* sphereRes = manager->getSphereResource();
	if (sphereRes->VB != nullptr)
	{
		for ( auto& planet : manager->GetPlanetList())
		{
			renderer->UpdateConstant(planet.GetTransformMatrix());
			//renderer->indexRenderPrimitive(sphereRes->VB, sphereRes->IB, sphereRes->IndexCount);
			renderer->textureRenderPrimitive(sphereRes->VB, sphereRes->IB, sphereRes->IndexCount, manager->GetResourceManager()->GetTexture(planet.getImageName()));
		}
	}

	
}

void InGameRunState::OnExit(UManager* manager)
{
	if (uiManager) {
		delete uiManager;
		uiManager = nullptr;
	}
}
#include "InGameRunState.h"
#include "UManager.h" // 매니저의 자원 접근을 위해 포함

#include "MainState.h"
#include "InGameReadyState.h"
#include "LoadingState.h"

#include<iostream>

void InGameRunState::OnEnter(UManager* manager)
{
	uiManager = new UIManager();

	UIFrame& bgFrame = uiManager->CreateFrame("MainState")
		.Position(ImVec2(0, 0))
		.Size(ImVec2(1400, 1050))
		.NoTitleBar(true)
		.BackgroundColor(ImVec4(0, 0, 0, 0));


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

	HUDFrame.AddSelectableText("Timer", "Remain Time : " + std::to_string(manager->GetRemainTimer()),
		ImVec2(WindowWidth * 3 / 4.f + 150, 500),
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

	hudFrame = &HUDFrame;

	manager->GetProbe()->ResetTrail();
}


bool InGameRunState::borderCheck(UManager* manager)
{
	Probe* player = manager->GetProbe();
	FVector location = player->GetLocation();
	float radius = player->GetRadius();

	if (-1.7f + radius > location.x or 1.2f - radius < location.x or -1.7f + radius > location.y or 1.7 - radius < location.y)
	{
		return true;
	}
	
	return false;
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

	if (remainTime <= 25.f)
	{
		manager->SetSuccess(false);

		//std::cout << "Time out" << std::endl;

		EndingState* endingState = new EndingState();
		//endingState->OnStageResult(false, manager->GetRemainTimer(), manager->GetCurStageInt());
		return endingState;
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

	auto pos = player->GetLocation();
	auto vel = player->GetVelocity();

	auto curLocation = player->GetLocation();
	player->SetLocation(curLocation+ player->GetVelocity() * deltaTime);
	//manager->GetProbe()->TryAddTrail(vel.Size() * vel.Size() * 2);
	manager->GetProbe()->TryAddTrail();

	FVector pLoc = player->GetLocation();

	//	Collision between Probe and Planets
	for (const auto& p : planetList)
	{
		float dist = (p.GetLocation() - pLoc).Size();
		if (dist < p.GetRadius() + player->GetRadius())
		{
			player->SetColliding(true);
			manager->SetSuccess(false);

			EndingState* endingState = new EndingState();
			//endingState->OnStageResult(false, manager->GetRemainTimer(), manager->GetCurStageInt());

			return endingState;
		}
	}

	//collision border with rocket
	if (borderCheck(manager))
	{
		EndingState* endingState = new EndingState();
		//endingState->OnStageResult(false, manager->GetRemainTimer(), manager->GetCurStageInt());

		return endingState;
	}


	if (bGoToMain)
	{
		nextState = new MainState();
	}
	else if (bGoToRetry)
	{

		std::cout << "bGoToRetry" << std::endl;
		nextState = new LoadingState();
	}

	

	return nextState;
}

void InGameRunState::Render(URenderer* renderer, UManager* manager)
{
	if (uiManager) uiManager->Render();

	renderer->UpdateConstant(manager->getGoal()->GetTransformMatrix());
	MeshResource* goal = manager->getGoalResource();
	if (goal != nullptr) {
		renderer->textureRenderPrimitive(goal->VB, goal->IB, goal->IndexCount, manager->GetResourceManager()->GetTexture(ImageName::GOAL));
	}
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

		for (auto& trail : manager->GetProbe()->GetTrails()) 
		{
			renderer->UpdateConstant(trail.GetTransformMatrix());
			renderer->textureRenderPrimitive(sphereRes->VB, sphereRes->IB, sphereRes->IndexCount, manager->GetResourceManager()->GetTexture("JustWhite"));
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

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
		manager->PlaySFX(ESFX::ESFX_Fail);

		//std::cout << "Time out" << std::endl;

		EndingState* endingState = new EndingState();
		//endingState->OnStageResult(false, manager->GetRemainTimer(), manager->GetCurStageInt());
		return endingState;
	}

	auto planetList = manager->GetPlanetList();
	auto player = manager->GetProbe();
	auto stageInfoList = manager->GetStageInfoList();   
	auto curStage = manager->GetCurStage();

	// 0. 완화 상수 설정
	const float epsilon = 0.2f;
	const float epsilonSq = epsilon * epsilon;

	for (auto p : planetList)
	{
		// 1. 방향 벡터 계산
		FVector direction = p.GetLocation() - player->GetLocation();

		// 2. SizeSquared 직접 계산 (x*x + y*y + z*z)
		// sqrt 연산을 거치지 않으므로 훨씬 빠르고 정밀합니다.
		float distSq = (direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z);

		// 0으로 나누기 방지 가드
		if (distSq < 1e-6f) continue;

		// 3. 가속도 계산 (소프닝 팩터 적용)
		float accMag = (GravitationalConstant * p.GetMass()) / (distSq + epsilonSq);

		// 4. 방향 벡터 적용을 위해 실제 거리 계산 (여기서만 sqrt 사용)
		float dist = sqrtf(distSq);
		FVector unitDir = direction / dist;
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

			manager->PlaySFX(ESFX::ESFX_Fail);
			return new EndingState();
		}
	}

	if (bGoToMain)
	{
		nextState = new MainState();
		return new EndingState();
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
	
}

void InGameRunState::OnExit(UManager* manager)
{
	if (uiManager) {
		delete uiManager;
		uiManager = nullptr;
	}
}
#include "MainState.h"
#include "ExampleState.h"
#include "UManager.h"

void MainState::OnEnter(UManager* manager)
{
	uiManager = new UIManager();
	UIFrame& frame1 = uiManager->CreateFrame("MainState")
		.Position(ImVec2(10, 10))
		.Size(ImVec2(200, 300));

	// Main State Initialization
	manager->ClearGameObjects();

	Probe* player = new Probe();
	player->SetLocation({ 0.0f, 0.0f, 0.0f });
	player->SetScale(0.1f);
	// manager->SetPlayer(player); // 이런 형태의 Setter가 필요할 수 있습니다.

	// 테스트용 행성(원) 생성 및 규격 설정
	USphere testPlanet;   
	testPlanet.SetLocation({ 0.5f, 0.0f, 0.0f });
	testPlanet.SetScale(0.1f);
	manager->CreateNewPlanetWorld(testPlanet);
}

IState* MainState::Update(float deltaTime, UManager* manager)
{
	nextState = this;

	// 메인 화면에서는 별다른 물리 로직이 필요 없으므로 업데이트를 생략하거나
	// 필요한 애니메이션 로직만 넣습니다.

	return nextState;
}

void MainState::Render(URenderer* renderer, UManager* manager)
{
	Probe* pPlayer = manager->GetProbe();
	if (pPlayer) {
		renderer->UpdateConstant(pPlayer->GetTransformMatrix());
		MeshResource* probeRes = manager->getProbeResource();
		if (probeRes->VB) {
			renderer->indexRenderPrimitive(probeRes->VB, probeRes->IB, probeRes->IndexCount);
		}
	}

	MeshResource* sphereRes = manager->getSphereResource();
	if (sphereRes->VB) {
		for (const auto& planet : manager->GetPlanetList()) {
			renderer->UpdateConstant(planet.GetTransformMatrix());
			renderer->indexRenderPrimitive(sphereRes->VB, sphereRes->IB, sphereRes->IndexCount);
		}
	}

	if (uiManager) uiManager->Render();
}

void MainState::OnExit(UManager* manager)
{
	if (uiManager) {
		delete uiManager;
		uiManager = nullptr;
	}
}
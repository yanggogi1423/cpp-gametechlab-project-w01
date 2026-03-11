#include "InGameReadyState.h"
#include "UManager.h" // 매니저의 자원 접근을 위해 포함

void InGameReadyState::OnEnter(UManager* manager)
{
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
		player->SetLocation({ 0.0f, -1.0f, 0.0f });
		player->SetVelocity({ 0.0f , 1.0f , 0.0f });
		goal.SetLocation({ 0.0f , 1.0f , 0.0f }); 
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
	UIFrame& readyFrame = uiManager->CreateFrame("Ready Phase")
		.Position(ImVec2(10, 10))
		.Size(ImVec2(200, 100));
}

IState* InGameReadyState::Update(float deltaTime, UManager* manager)
{
	nextState = this;
	// 준비 단계에서는 마우스 클릭으로 행성을 배치하는 로직 등을 추가할 수 있습니다.
	return nextState;
}


// texture 렌더링
void InGameReadyState::Render(URenderer* renderer, UManager* manager)
{

	// 1. 플레이어 렌더링
	Probe* pPlayer = manager->GetProbe();
	if (pPlayer) {
		renderer->UpdateConstant(pPlayer->GetTransformMatrix());
		MeshResource* res = manager->getProbeResource();
		renderer->indexRenderPrimitive(res->VB, res->IB, res->IndexCount);
		renderer->textureRenderPrimitive(res->VB, res->IB, res->IndexCount, manager->GetResourceManager()->GetTexture(ImageName::ROCKET));
	}

	// 2. 행성들 렌더링
	MeshResource* sphereRes = manager->getSphereResource();
	for (auto& planet : manager->GetPlanetList()) {
		renderer->UpdateConstant(planet.GetTransformMatrix());
		renderer->textureRenderPrimitive(sphereRes->VB, sphereRes->IB, sphereRes->IndexCount, manager->GetResourceManager()->GetTexture(planet.getImageName()));

	}

	if (uiManager) uiManager->Render();
}

void InGameReadyState::OnExit(UManager* manager)
{
	if (uiManager) {
		delete uiManager;
		uiManager = nullptr;
	}
}
#include "StageSelectionState.h"
#include "UManager.h"

void StageSelectionState::OnEnter(UManager* manager)
{
	// Stage Selection State Initialization
	manager->ClearGameObjects();

	// UI 관리자 생성 및 스테이지 선택 버튼 배치
	uiManager = new UIManager();
	UIFrame& selectFrame = uiManager->CreateFrame("Stage Select")
		.Position(ImVec2(312, 212)) // 화면 중앙 근처 배치
		.Size(ImVec2(400, 500));
}

IState* StageSelectionState::Update(float deltaTime, UManager* manager)
{
	nextState = this;

	// [참고] 스테이지가 선택되어 manager의 상태가 Loading으로 바뀌면,
	// 여기서 상태 전환(LoadingState로 이동 등)을 판단할 수 있습니다.

	return nextState;
}

void StageSelectionState::Render(URenderer* renderer, UManager* manager)
{
	// 스테이지 선택 화면 UI 출력
	if (uiManager) uiManager->Render();
}

void StageSelectionState::OnExit(UManager* manager)
{
	if (uiManager) {
		delete uiManager;
		uiManager = nullptr;
	}
}
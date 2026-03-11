#include "EndingState.h"
#include "UManager.h"

// 생성 시 결과 데이터를 저장합니다.
EndingState::EndingState(bool bIsClear, unsigned int score, std::string name)
	: m_bIsClear(bIsClear), m_score(score), m_name(name) {
}

void EndingState::OnEnter(UManager* manager)
{
	// 1. [이사 완료] 객체 정리
	manager->ClearGameObjects();

	// 2. [이사 완료] 성공/실패 사운드 재생
	// (참고: manager에 사운드 재생을 위한 public 함수가 필요합니다)
	if (m_bIsClear) manager->PlaySFX(ESFX::ESFX_Clear);
	else manager->PlaySFX(ESFX::ESFX_Fail);

	// 3. [이사 완료] 점수 기록 및 랭킹 업데이트
	manager->DisplayScore(m_name, m_score);

	// 4. 엔딩 UI 생성 (다시 하기, 메인으로 등)
	uiManager = new UIManager();
	// ... UI 구성 로직
}

IState* EndingState::Update(float deltaTime, UManager* manager)
{
	nextState = this;
	return nextState;
}

void EndingState::Render(URenderer* renderer, UManager* manager)
{
	// 엔딩 화면 UI 출력
	if (uiManager) uiManager->Render();
}

void EndingState::OnExit(UManager* manager)
{
	if (uiManager) {
		delete uiManager;
		uiManager = nullptr;
	}
}
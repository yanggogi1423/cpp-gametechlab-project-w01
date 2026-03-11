#include "InGameRunState.h"
#include "UManager.h" // 매니저의 자원 접근을 위해 포함

void InGameRunState::OnEnter(UManager* manager)
{
	// 1. [이사 완료] UManager::InGameRunInit 로직
	// 이제 이 상태에 진입한 것 자체가 'InGameRun'이 시작되었음을 의미합니다.
}

IState* InGameRunState::Update(float deltaTime, UManager* manager)
{
	nextState = this;

	// 2. [이사 완료] UManager::Update에 있던 물리 엔진 가동
	// 중력 계산 및 속도 적용
	manager->ComputePhysicsAndApply(deltaTime);

	// 충돌 및 승리/패배 판정
	manager->CollisionDetection();

	// 3. 타이머 업데이트 (ReadyState에서 설정된 시간 소모)
	float currentTimer = manager->GetRemainTimer(); // Getter 필요
	manager->SetRemainTimer(currentTimer - deltaTime); // Setter 필요

	// 만약 시간이 다 되면 EndingState로 전환하는 로직을 여기서 결정할 수 있습니다.

	return nextState;
}

void InGameRunState::Render(URenderer* renderer, UManager* manager)
{
	// 4. [이사 완료] main.cpp에 있던 3D 객체 렌더링 루프

	// (1) 플레이어(Probe) 렌더링
	Probe* pPlayer = manager->GetProbe();
	if (pPlayer != nullptr)
	{
		renderer->UpdateConstant(pPlayer->GetTransformMatrix());
		MeshResource* probeRes = manager->getProbeResource();
		if (probeRes->VB != nullptr)
		{
			renderer->indexRenderPrimitive(probeRes->VB, probeRes->IB, probeRes->IndexCount);
		}
	}

	// (2) 행성(Sphere)들 렌더링 (공유 버퍼 활용)
	MeshResource* sphereRes = manager->getSphereResource();
	if (sphereRes->VB != nullptr)
	{
		for ( auto& planet : manager->GetPlanetList())
		{
			renderer->UpdateConstant(planet.GetTransformMatrix());
			renderer->indexRenderPrimitive(sphereRes->VB, sphereRes->IB, sphereRes->IndexCount);
		}
	}

	// (3) UI 렌더링 (필요 시)
	if (uiManager) uiManager->Render();
}

void InGameRunState::OnExit(UManager* manager)
{
	// 공정 종료 시 필요한 정리 작업
	if (uiManager) {
		delete uiManager;
		uiManager = nullptr;
	}
}
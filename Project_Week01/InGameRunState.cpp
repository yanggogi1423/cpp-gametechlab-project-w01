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
			manager->PlaySFX(ESFX::ESFX_Clear);
			return new EndingState();
		}
	}

	// Collision between Probe and Exit Location
	float goalDist = (stageInfoList[(int)curStage - 1].goal.GetLocation() - pLoc).Size();
	if (goalDist < 0.15f)
	{
		manager->SetSuccess(true);
		manager->PlaySFX(ESFX::ESFX_Fail);
		return new EndingState();
	}

	// 3. 타이머 업데이트 (ReadyState에서 설정된 시간 소모)
	float remainTime = manager->GetRemainTimer(); // Getter 필요

	remainTime = remainTime - deltaTime;
	remainTime = max(0.f, remainTime);
	manager->SetRemainTimer(remainTime); // Setter 필요

	if (remainTime <= 0.f)
	{
		manager->SetSuccess(false);
		return new EndingState();
	}

	 return nextState;
}

void InGameRunState::Render(URenderer* renderer, UManager* manager)
{
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

	MeshResource* sphereRes = manager->getSphereResource();
	if (sphereRes->VB != nullptr)
	{
		for ( auto& planet : manager->GetPlanetList())
		{
			renderer->UpdateConstant(planet.GetTransformMatrix());
			renderer->indexRenderPrimitive(sphereRes->VB, sphereRes->IB, sphereRes->IndexCount);
		}
	}

	if (uiManager) uiManager->Render();
}

void InGameRunState::OnExit(UManager* manager)
{
	if (uiManager) {
		delete uiManager;
		uiManager = nullptr;
	}
}
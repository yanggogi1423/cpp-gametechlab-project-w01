#pragma once
#include "IState.h"
#include "Probe.h"
#include "USphere.h"
#include "UManager.h" //FStageInfo, EStage

class InGameRunState : public IState
{
public:
	InGameRunState() = default;
	virtual ~InGameRunState() = default;

	void OnEnter(class UManager* manager) override;
	IState* Update(float deltaTime, class UManager* manager) override;
	void Render(class URenderer* renderer, class UManager* manager) override;
	void OnExit(class UManager* manager) override;

private:
	void ComputePhysicsAndApply(float deltaTime, std::vector<USphere> planetList, Probe* player);
	void CollisionDetection(std::vector<USphere> planetList, Probe* player, std::vector<FStageInfo> stageInfoList, EStage curStage);
};


#pragma once
#include <fstream>
#include <string>
#include <sstream>


#include "IState.h"
#include "InGameReadyState.h"
#include "StageSelectionState.h"
class EndingState : public IState
{
private:
	int curStage;
	const std::string FileName;
	std::vector<std::tuple<unsigned int, std::string, unsigned int>> ScoreList;
	UResourceManager* ResourceManager;
	bool isSuccess;
	float RemainTimer;

	std::string RandomNameGenerator();
	void EndingInit(bool bIsClear, unsigned int score, std::string name);
	void DisplayScore(std::string name, unsigned int score);
	void LoadScore();
	void SaveScore();
	void ShutDownGame();
	void OnStageResult(bool bSuccess);
	//void ClearGameObjects();
	//void ProgressStage();

public:
	EndingState(int stage, UResourceManager* resourceManager, bool bSuccess, float remainTimer);
	virtual ~EndingState() = default;
	void OnEnter() override;
	IState* Update(URenderer* renderer) override;
	void OnExit() override;
};
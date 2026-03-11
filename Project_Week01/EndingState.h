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
	const std::string FileName;
	std::vector<std::tuple<unsigned int, std::string, unsigned int>> ScoreList;
	UResourceManager* ResourceManager;

	std::string RandomNameGenerator();
	void EndingInit(bool bIsClear, unsigned int score, std::string name, int);
	void DisplayScore(std::string name, unsigned int score, int);
	void LoadScore();
	void SaveScore();
	void ShutDownGame();
	void OnStageResult(bool bSuccess, float, int);
	//void ClearGameObjects();
	//void ProgressStage();

	bool goToMain = false;
	bool retry = false;
	bool stageSelect = false;

public:
	EndingState();
	virtual ~EndingState() = default;
	void OnEnter(UManager* manager) override;
	IState* Update(float deltaTime, UManager* manager) override;
	void Render(URenderer* renderer, UManager* manager) override;
	void OnExit(UManager* manager) override;
};
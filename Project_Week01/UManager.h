#pragma once

#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

//	Constants
constexpr float TopBorder = -1.f;
constexpr float BottomBorder = 1.f;
constexpr float LeftBorder = -1.f;
constexpr float TopBorder = 1.f;

#pragma region __GAME_STATE__

//	None 상태를 갖지 않음
enum class ERunstate
{
	ERS_Main,
	ERS_InGame,
	ERS_Ending	//	플레이어의 사망 || 게임 클리어
};

enum class EStage
{
	ES_None,
	ES_Stage1,
	ES_Stage2,
	ES_Stage3
};

#pragma endregion


class UManager
{
private:
	//	Game State
	ERunstate CurRunState;
	EStage CurStage;

	//	Time
	float RemainTime;

private:
	const std::string FileName;
	std::vector<std::pair<std::string, unsigned int>> ScoreList;

	/* GameObjects */
public:
	//	행성의 종류가 dynamic할 수 있음을 염두

	/* Game Management */
private:
	//	Collision 관련
	void CollisionDetection();
	void CollsionResolution();

	//	Scene Initialization
	void MainInit();
	void InGameInit();
	void EndingInit();

	//	Stage Progress
	void ProgressStage();
	void InitGameObjects();
	void ClearGameObjects();

	/* Non-game Management */
	//	File Load
	void LoadScore();
	void SaveScore();
	void DisplayScore(std::string name, unsigned int score);


public:
	UManager()
		: CurRunState(ERunstate::ERS_Main), CurStage(EStage::ES_None),
		FileName("ranking.txt")
	{
		LoadScore();
	}
	~UManager()
	{
		SaveScore();
	}

	void Update(float deltaTime);


};
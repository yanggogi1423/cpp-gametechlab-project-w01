#pragma once

#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

#include "Probe.h"
#include "datatype.h"


//	Constants
constexpr float TopBorder = -1.f;
constexpr float BottomBorder = 1.f;
constexpr float LeftBorder = -1.f;
constexpr float RightBorder = 1.f;

constexpr size_t PlanetListReservedSize = 10;

#pragma region __GAME_STATE__

//	None 상태를 갖지 않음
enum class ERunstate
{
	ERS_Main,
	ERS_InGameReady,
	ERS_InGameRun,
	ERS_Ending	//	플레이어의 사망 || 게임 클리어
};

#pragma endregion

#pragma region __STAGE_INFO__

enum class EStage
{
	ES_None,
	ES_Stage1,
	ES_Stage2,
	ES_Stage3
};

//	스테이지에 대한 정보 저장
struct FStageInfo
{
	unsigned int Index;
	float MaxTime;
	//	1. 행성의 종류(idx 구분)와 개수에 대한 정보
	//	2. 장애물 위치 및 종류(idx), 개수 정보 -> 위치, 개수를 pair (혹은 개수만)
	//	3. 탈출구 위치
};

#pragma endregion


class UManager
{
private:
	//	Game State
	ERunstate CurRunState;
	EStage CurStage;

	//	Time
	float RemainTimer;

	//	Boot and Destroy - 반복 호출을 막기 위한 flag
	bool bBootDone;
	bool bIsAlreadyDestroy;

private:
	const std::string FileName;
	std::vector<std::pair<std::string, unsigned int>> ScoreList;

	/* GameObjects */
	Probe* Player;
	std::vector<UPrimitive*> PlanetList;	//	이후에 template 수정할 수도 있음


	

	/* Game Management */
private:
	//	Collision 관련
	void CollisionDetection();
	void CollsionResolution();

	//	Scene Initialization
	void MainInit();		//	Opening(시작 화면)으로 분기
	void InGameReadyInit();	//	행성 배치 가능 상태로 분기
	void InGameRunInit();	//	사실상 Simulation Start
	void EndingInit(bool bIsClear);		//	Clear 혹은 사망 판정 시 호출 및 분기

	//	Stage Progress
	void ProgressStage();
	void InitGameObjects();
	void ClearGameObjects();

	//	Game Logic
	//	행성 배치


	/* Non-game Management */
	void BootGame();	//	Application 실행 시 호출 (게임 데이터 준비)
	void DestroyGame();	//	Application 종료 시 호출 (게임 데이터 정리 및 저장)

	//	File Load
	void LoadScore();
	void SaveScore();
	void DisplayScore(std::string name, unsigned int score);


public:
	/* Cons, Des */
	UManager()
		: CurRunState(ERunstate::ERS_Main), CurStage(EStage::ES_None),
		FileName("ranking.txt"),
		bBootDone(false), bIsAlreadyDestroy(false)
	{
		BootGame();
	}
	~UManager()
	{
		DestroyGame();
	}

	void Update(float deltaTime);

	/* Getter, Setter */
	const Probe& GetProbe() const { return (*Player);  }
	bool Startable() const { return bBootDone; }

};
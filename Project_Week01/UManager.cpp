#include "UManager.h"

//	Private Functions

/* Game Management */
void UManager::CollisionDetection()
{
	//	1. Collision between Probe and Border
	//	Probe가 triangle이면 각 vertex의 정보를 담고 있어야 Detection이 가능

	//	2. Collision between Probe and Planets


}

//	어쩌면 Resolution이 필요없을 수도? (게임 오버)
void UManager::CollsionResolution()
{

}

void UManager::MainInit()
{
	CurRunState = ERunstate::ERS_Main;

	ClearGameObjects();
}

void UManager::InGameReadyInit()
{
	CurRunState = ERunstate::ERS_InGameReady;

	ClearGameObjects();
}

void UManager::InGameRunInit()
{
	CurRunState = ERunstate::ERS_InGameRun;

	ClearGameObjects();
}

void UManager::EndingInit()
{
	CurRunState = ERunstate::ERS_Ending;

	ClearGameObjects();
}

void UManager::ProgressStage()
{
	if (CurRunState != ERunstate::ERS_InGameReady)
	{
		//	TODO : Make an error log

		return;
	}

	switch (CurStage)
	{
	case EStage::ES_Stage1:
		break;

	case EStage::ES_Stage2:
		break;

	case EStage::ES_Stage3:
		break;
	}

	ClearGameObjects();
}

//	InGameReady 상태로 분기 시 플레이어 및 장애물 생성
void UManager::InitGameObjects()
{
	Player = new Probe();

	//	TODO : 장애물 생성 로직
}

//	모든 오브젝트 삭제
//	RunState와 Stage 상태 변동시 항상 적용
void UManager::ClearGameObjects()
{
	if (Player)
	{
		delete Player;
	}
	Player = nullptr;

	if (!PlanetList.empty())
	{
		PlanetList.clear();
	}
	//	Reserve size
	PlanetList.reserve(PlanetListReservedSize);
}

void UManager::BootGame()
{
	if (bBootDone)
	{
		//	TODO : Make an error log
		return;
	}

	LoadScore();

	//	1. 스테이지 정보 생성

	//	2. 
}

//	외부에서 호출해줘야 함 (혹은 UManager Destructor에서 호출됨)
void UManager::DestroyGame()
{
	if (bIsAlreadyDestroy) {
		//	TODO : Make an error log
		return;
	}

	SaveScore();

	//	1. Heap 해제
	
	bIsAlreadyDestroy = true;
}

/* Non-game Management */
//	DataStruct : <nickname>,<score> (CSV-like based txt)
//	실행 시에만 Load
void UManager::LoadScore()
{
	ScoreList.clear();

	std::ifstream ifs(FileName);
	std::string line;

	while (std::getline(ifs, line))
	{
		std::stringstream ss(line);

		std::string name;
		unsigned int score;

		//	구분자 : ,
		std::getline(ss, name, ',');
		ss >> score;

		ScoreList.push_back({ name, score });
	}

	ifs.close();
}

//	Vector 정렬 후 File 형식에 맞추어 Parsing
void UManager::SaveScore()
{
	std::ofstream ofs(FileName);

	for (auto s : ScoreList)
	{
		ofs << s.first << "," << s.second << "\n";
	}

	ofs.close();
}

//	File이 아닌 Runtime Vector를 통해 읽어옴
void UManager::DisplayScore(std::string name, unsigned int score)
{
	//	List에 포함해서 보여주기 (포함 후 정렬 -> 보여주기)
	ScoreList.push_back({ name, score });
	std::sort(ScoreList.begin(), ScoreList.end(), [](const std::pair<std::string, unsigned int>& a, const std::pair<std::string, unsigned int>& b)
		{
			return a.second < b.second;
		}
	);

	//	TODO : Imgui를 통해 display
}


//	Public Functions
void UManager::Update(float deltaTime)
{
	switch (CurRunState)
	{
	case ERunstate::ERS_Main:
		break;
	case ERunstate::ERS_InGameReady:
		
		break;
	case ERunstate::ERS_InGameRun:
		CollisionDetection();	//	필요하다면 반복
		break;
	case ERunstate::ERS_Ending:
		break;
	default:
		break;
	}

}


#include "UManager.h"

//	Private Functions

/* Game Management */
void UManager::CollisionDetection()
{

}

void UManager::CollsionResolution()
{

}

void UManager::MainInit()
{
	CurRunState = ERunstate::ERS_Main;
}

void UManager::InGameInit()
{
	CurRunState = ERunstate::ERS_Main;
}

void UManager::EndingInit()
{
	CurRunState = ERunstate::ERS_Ending;
}

void UManager::ProgressStage()
{
	if (CurRunState != ERunstate::ERS_InGame)
	{
		//	TODO : Debug 용 Log 생성

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

}

//	모든 오브젝트 생성 - Start 버튼 클릭 시
void InitGameObjects()
{

}

//	모든 오브젝트 삭제 - Ending 상태로 분기 시
void ClearGameObjects()
{

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


}
#include "EndingState.h"
#include "UManager.h"

EndingState::EndingState(int stage, bool bSuccess, float remainTimer) :
	curStage(stage),
	FileName("ranking.txt"),
	isSuccess(bSuccess),
	RemainTimer(remainTimer)
{
}

void EndingState::OnEnter(UManager* manager)
{
	ResourceManager = manager->GetResourceManager();
	OnStageResult(isSuccess);

	UIFrame& scoreFrame = uiManager->CreateFrame("Score")
		.Position(ImVec2(100, 100))
		.Size(ImVec2(500, 200));

	const int displayCount = 10;
	int maxCount = min(displayCount, ScoreList.size());

	for (int i = 0; i < maxCount; i++)
	{
		auto name = std::get<1>(ScoreList[i]);
		scoreFrame.AddText(name, ImVec2(30, i * 20.f), ResourceManager->FontDefault);
	}

	UIFrame& buttonFrame = uiManager->CreateFrame("Button")
		.Position(ImVec2(100, 300))
		.Size(ImVec2(500, 200));

	buttonFrame.AddButton("ShutDown", ImVec2(50, 50), ImVec2(50, 50), [&] {ShutDownGame(); });
	buttonFrame.AddButton("Retry", ImVec2(50, 150), ImVec2(50, 50), [&] {nextState = new InGameReadyState(); });
	buttonFrame.AddButton("StageSelect", ImVec2(50, 250), ImVec2(50, 50), [&] {nextState = new StageSelectionState(); });
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
	delete uiManager;
}


std::string EndingState::RandomNameGenerator()
{
	const std::string charSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	const int appendLength = 8;

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, charSet.size() - 1);

	std::string name = "User_";

	for (int i = 0; i < appendLength; i++)
	{
		name += charSet[dist(mt)];
	}

	return name;
}

//void EndingState::ClearGameObjects()
//{
//	if (Player)
//	{
//		delete Player;
//		Player = nullptr;
//	}
//
//	PlanetList.clear();
//
//	//	Reserve size
//	PlanetList.reserve(PlanetListReservedSize);
//}
//
//void EndingState::ProgressStage()
//{
//	//switch (CurAvailableStage)
//	//{
//	//case EStage::ES_Stage1:
//	//	CurAvailableStage = EStage::ES_Stage2;
//	//	break;
//
//	//case EStage::ES_Stage2:
//	//	CurAvailableStage = EStage::ES_Stage3;
//	//	break;
//
//	//case EStage::ES_Stage3:
//	//	//	Do nothing
//	//	break;
//	//}
//
//	ClearGameObjects();
//}

// 결과 화면 시 호출될 함수
void EndingState::OnStageResult(bool bSuccess)
{
	//ProgressStage();

	unsigned int finalScore = 0;
	if (bSuccess) finalScore = (unsigned int)(RemainTimer * RemainTimer);
	else finalScore = 0;

	EndingInit(bSuccess, finalScore, RandomNameGenerator());
}

void EndingState::EndingInit(bool bIsClear, unsigned int score, std::string name)
{
	//ClearGameObjects(); InGameRunOnExit에서

	if (bIsClear)
	{
		//m_SoundMgr.PlaySFX(ESFX::ESFX_Clear);
	}
	else
	{
		//m_SoundMgr.PlaySFX(ESFX::ESFX_Fail);
	}



	//	Score Display
	DisplayScore(name, score);
}

void EndingState::DisplayScore(std::string name, unsigned int score)
{
	//	List에 포함해서 보여주기 (포함 후 정렬 -> 보여주기)
	ScoreList.push_back({ curStage, name, score });
	std::sort(ScoreList.begin(), ScoreList.end(), [](const auto& a, const auto& b)
		{
			//	다른 스테이지일 경우 Sort하지 않음
			//	Display 시에도 Stage로 filtering하면 됨
			if (std::get<0>(a) != std::get<0>(b)) return false;

			return std::get<2>(a) < std::get<2>(b);
		}
	);

	//	TODO : Imgui를 통해 display
}

void EndingState::LoadScore()
{
	ScoreList.clear();

	std::ifstream ifs(FileName);
	std::string line;


	while (std::getline(ifs, line))
	{
		std::stringstream ss(line);

		unsigned int stage;
		std::string name;
		unsigned int score;

		char dummy;

		//	구분자 : ,
		ss >> stage;
		ss >> dummy;	//	첫 번째 ',' 가짐
		std::getline(ss, name, ',');
		ss >> score;

		ScoreList.push_back({ stage, name, score });
	}

	ifs.close();
}


void EndingState::ShutDownGame()
{
	SaveScore();



	//	1. Heap 해제
	ResourceManager->Release();
}

void EndingState::SaveScore()
{
	std::ofstream ofs(FileName);
	// ScoreList 튜플 구조 <Stage, Name, Score>에 맞춰 저장합니다.
	for (const auto& s : ScoreList)
	{
		ofs << std::get<0>(s) << "," << std::get<1>(s) << "," << std::get<2>(s) << "\n";
	}
	ofs.close();
}
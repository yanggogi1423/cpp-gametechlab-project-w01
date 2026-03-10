#include "UManager.h"

//	Private Functions

/* Game Management */
void UManager::CollisionDetection()
{
	if (!Player) return;
	
	//	1. Collision between Probe and Border
	//	Probe가 triangle이면 각 vertex의 정보를 담고 있어야 Detection이 가능

	//	2. Collision between Probe and Planets
}

//	어쩌면 Resolution이 필요없을 수도? (게임 오버)
void UManager::CollsionResolution()
{
	//EndingInit(false);
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

	InitGameObjects();
}

//	Stage를 클리어하거나, 플레이어가 사망했을 때 호출
//	bIsClear는 클리어 여부를 담음. (main에서 호출됨) -> default parameter를 고려하여 설계
//	name은 사용자에게 직접 입력 받습니다.
void UManager::EndingInit(bool bIsClear, unsigned int score, std::string name)
{
	CurRunState = ERunstate::ERS_Ending;

	ClearGameObjects();

	if (bIsClear)
	{

	}
	else 
	{

	}

	//	Score Display
	DisplayScore(name, score);
}

void UManager::ProgressStage()
{
	if (CurRunState != ERunstate::ERS_InGameReady)
	{
		//	TODO : Make an error log

		return;
	}

	switch (CurAvailableStage)
	{
	case EStage::ES_Stage1:
		CurAvailableStage = EStage::ES_Stage2;
		break;

	case EStage::ES_Stage2:
		CurAvailableStage = EStage::ES_Stage3;
		break;

	case EStage::ES_Stage3:
		//	Do nothing
		break;
	}

	ClearGameObjects();
}
// probe sphere initialize




// TODO 수정해야함
//	InGameReady 상태로 분기 시 플레이어 및 장애물 생성
void UManager::InitGameObjects()
{
	Player = new Probe();
	Player->SetLocation({ 0.0f, -0.8f, 0.0f });
	
	USphere* TestPlanet = new USphere();
	TestPlanet->SetLocation({ 0.5f, 0.8f, 0.0f });
	TestPlanet->SetVelocity({ 0.0f, -0.2f, 0.0f });

	PlanetList.push_back(TestPlanet);

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

	for (auto& planet : PlanetList)
	{
		if (planet) delete planet;
	}

	PlanetList.clear();

	//	Reserve size
	PlanetList.reserve(PlanetListReservedSize);
}

void UManager::ComputePhysicsAndApply(float deltaTime)
{
	for (auto p : PlanetList)
	{
		float dist = (p->GetLocation() - Player->GetLocation()).Size();
		FVector acc = (GravititationalConstant * p->GetMass()) / pow(dist, 2);

		Player->SetVelocity(Player->GetVelocity() + acc * deltaTime);
	}
}

void UManager::BootGame()
{
	if (CurRunState != ERunstate::ERS_Boot)
	{
		//	TODO : Make an error log
		return;
	}
	
	//	1. Local Score 로드
	LoadScore();

	//	2. 스테이지 정보 생성
	StageInfoList.push_back({ EStage::ES_Stage1,30.f });
	StageInfoList.push_back({ EStage::ES_Stage2,30.f });
	StageInfoList.push_back({ EStage::ES_Stage3,30.f });

	//	3. 메인 State로 분기
	MainInit();

}

//	외부에서 호출해줘야 함 (혹은 UManager Destructor에서 호출됨)
void UManager::ShutDownGame()
{
	if (CurRunState == ERunstate::ERS_Destroy) 
	{
		//	TODO : Make an error log
		return;
	}

	SaveScore();



	//	1. Heap 해제
	
	CurRunState == ERunstate::ERS_Destroy;
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
		//	Value Input
		ComputePhysicsAndApply(deltaTime);

		//	Physics Update
		Player->SetLocation(Player->GetLocation() + Player->GetVelocity() * deltaTime);

		CollisionDetection();	//	필요하다면 반복
		break;
	case ERunstate::ERS_Ending:
		break;
	default:
		break;
	}

}

void UManager::initResource(RESOURCE_TYPE rt, ID3D11Buffer* vb, ID3D11Buffer* ib, unsigned int vertexCount, unsigned int indexCount, unsigned int stride)
{

	switch (rt)
	{
	case PROBE:
		ProbeResource.initResource(vb, ib, vertexCount, indexCount, stride);
		break;
	case SPHERE:
		SphereResource.initResource(vb, ib, vertexCount, indexCount, stride);
		break;
	default:
		break;
	}

}

MeshResource UManager::getProbeResource() const
{	
	return ProbeResource;
}
MeshResource UManager::getSphereResource() const
{	
	return SphereResource;
}

void UManager::setProbeResource(const MeshResource& mr)
{
	this->ProbeResource = mr;
}
void UManager::setSphereResource(const MeshResource& mr)
{
	this->SphereResource = mr;
}




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
	ClearGameObjects();
	
	CurRunState = ERunstate::ERS_Main;
}

void UManager::InGameReadyInit()
{
	ClearGameObjects();

	CurRunState = ERunstate::ERS_InGameReady;
}

void UManager::InGameRunInit()
{
	ClearGameObjects();

	InitGameObjects();

	CurRunState = ERunstate::ERS_InGameRun;
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

void UManager::BootGame(ID3D11Device * device)
{
	if (CurRunState != ERunstate::ERS_Boot)
	{
		//	TODO : Make an error log
		return;
	}
	
	//	1. Local Score 및 ResourceManager 로드
	LoadScore();
	ResourceManager->Initialize(device);

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
	ResourceManager->Release();

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

void UManager::initResource(RESOURCE_TYPE rt, ID3D11Buffer* vb, ID3D11Buffer* ib, unsigned int vertexCount, unsigned int indexCount, unsigned int stride, float scale)
{

	switch (rt)
	{
	case PROBE:
		ProbeResource.initResource(vb, ib, vertexCount, indexCount, stride ,scale);
		break;
	case SPHERE:
		SphereResource.initResource(vb, ib, vertexCount, indexCount, stride,scale);
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

void GenerateVertices::GenerateTriangle(std::vector<FVertex>& outVertices, std::vector<unsigned int>& outIndices)
{
	outVertices = {
		// Position          // Color (RGBA)
		{  0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f }, // 위 (빨강)
		{  0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f }, // 오른쪽 아래 (초록)
		{ -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f }  // 왼쪽 아래 (파랑)
	};

	outIndices = { 0, 1, 2 };
}

void GenerateVertices::GenerateSphere(float radius, std::vector<FVertex>& outVertices, std::vector<unsigned int>& outIndices)
{
	// 적절한 정밀도 설정 (값이 클수록 매끄럽지만 계산량이 늘어남)
	const uint32_t sliceCount = 20;
	const uint32_t stackCount = 20;

	outVertices.clear();
	outIndices.clear();

	// 1. 정점(Vertex) 생성
	for (uint32_t i = 0; i <= stackCount; ++i) {
		float phi = 3.1415926535f * i / stackCount;
		for (uint32_t j = 0; j <= sliceCount; ++j) {
			float theta = 2.0f * 3.1415926535f * j / sliceCount;

			FVertex v;
			v.x = radius * sinf(phi) * cosf(theta);
			v.y = radius * cosf(phi);
			v.z = radius * sinf(phi) * sinf(theta);

			// 색상: 좌표 기반 (디버깅 시 구의 입체감을 확인하기 좋음)
			v.r = (v.x / radius) * 0.5f + 0.5f;
			v.g = (v.y / radius) * 0.5f + 0.5f;
			v.b = (v.z / radius) * 0.5f + 0.5f;
			v.a = 1.0f;

			outVertices.push_back(v);
		}
	}

	// 2. 인덱스(Index) 생성
	for (uint32_t i = 0; i < stackCount; ++i) {
		for (uint32_t j = 0; j < sliceCount; ++j) {
			uint32_t first = i * (sliceCount + 1) + j;
			uint32_t second = first + sliceCount + 1;

			// 삼각형 1
			outIndices.push_back(second);
			outIndices.push_back(first + 1);
			outIndices.push_back(first);

			// 삼각형 2
			outIndices.push_back(second + 1);
			outIndices.push_back(first + 1);
			outIndices.push_back(second);
		}
	}
}
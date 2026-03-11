#include "UManager.h"
#include "UPrimitive.h"
#include "Probe.h"
#include "USphere.h"
#include "datatype.h"
#include "USoundManager.h"
#include "MainState.h"
#include "InGameReadyState.h"
#include "InGameRunState.h"
#include "EndingState.h"
#include "LoadingState.h"
#include "BootState.h"

//	Private Functions

/* Game Management */
void UManager::CollisionDetection()
{
	if (!Player) return;

	FVector pLoc = Player->GetLocation();
	
	//	Collision between Probe and Planets
	for (const auto& p : PlanetList)
	{
		float dist = (p.GetLocation() - pLoc).Size();
		if (dist < p.GetScale() + Player->GetScale())
		{
			Player->SetColliding(true);
			//OnStageResult(false);
			return;
		}
	}

	// Collision between Probe and Exit Location
	float goalDist = (StageInfoList[(int)CurStage - 1].ExitLocation - pLoc).Size();
	if (goalDist < 0.15f)
	{
		//OnStageResult(true);
	}
}

//	어쩌면 Resolution이 필요없을 수도? (게임 오버)
void UManager::CollsionResolution()
{
	//EndingInit(false);
} 

//	모든 오브젝트 삭제
//	RunState와 Stage 상태 변동시 항상 적용
void UManager::ClearGameObjects()
{
	if (Player)
	{ 
		delete Player; 
		Player = nullptr;
	}

	PlanetList.clear();

	//	Reserve size
	PlanetList.reserve(PlanetListReservedSize);
}

void UManager::ComputePhysicsAndApply(float deltaTime)
{
	for (auto p : PlanetList)
	{
		// 1. 방향 벡터 및 거리 계산
		FVector direction = p.GetLocation() - Player->GetLocation();
		float dist = direction.Size();
		if (dist < 1e-4f) continue; // 0으로 나누기 방지

		// 2. 가속도 크기 및 방향 벡터(단위 벡터) 계산
		FVector unitDir = direction / dist;
		float accMag = (GravititationalConstant * p.GetMass()) / (float)pow(dist, 2);
		FVector accVec = unitDir * accMag;

		// 3. 속도 업데이트
		Player->SetVelocity(Player->GetVelocity() + accVec * deltaTime);
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
	ResourceManager = new UResourceManager();
	ResourceManager->Initialize(device);

	InputManager = new PlayerInput();

	//	2. 스테이지 정보 생성
	StageInfoList.push_back({ EStage::ES_Stage1,30.f });
	StageInfoList.push_back({ EStage::ES_Stage2,30.f });
	StageInfoList.push_back({ EStage::ES_Stage3,30.f });

	//	3. 메인 State로 분기

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

	CurRunState = ERunstate::ERS_Destroy;
}

/* Non-game Management */
//	DataStruct : <stage>,<nickname>,<score> (CSV-like based txt)
//	실행 시에만 Load
void UManager::LoadScore()
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

//	Vector 정렬 후 File 형식에 맞추어 Parsing
//	이는 Shutdown
void UManager::SaveScore()
{
	std::ofstream ofs(FileName);
	// ScoreList 튜플 구조 <Stage, Name, Score>에 맞춰 저장합니다.
	for (const auto& s : ScoreList)
	{
		ofs << std::get<0>(s) << "," << std::get<1>(s) << "," << std::get<2>(s) << "\n";
	}
	ofs.close();
}

//	File이 아닌 Runtime Vector를 통해 읽어옴
void UManager::DisplayScore(std::string name, unsigned int score)
{
	int stage = -1;
	switch (CurStage)
	{
	case EStage::ES_Stage1:
		stage = 1;
		break;
	case EStage::ES_Stage2:
		stage = 2;
		break;
	case EStage::ES_Stage3:
		stage = 3;
		break;
	default:
		break;
	}
	//	List에 포함해서 보여주기 (포함 후 정렬 -> 보여주기)
	ScoreList.push_back({ stage, name, score });
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


void UManager::CreateNewPlanetWorld(USphere& in)
{
	PlanetList.push_back(in);	//	이후에 Reference
}

//	Public Functions
void UManager::Initialize(HWND hwnd) // 사운드 초기화
{
	m_SoundMgr.Initialize(hwnd);
	m_SoundMgr.LoadBGM(EBGM::EBGM_Main, "Sound/Level1.wav");
	m_SoundMgr.LoadSFX(ESFX::ESFX_MouseClick, "Sound/MouseClick.wav", 5);
	m_SoundMgr.LoadSFX(ESFX::ESFX_Clear, "Sound/Clear.wav", 5);
	m_SoundMgr.LoadSFX(ESFX::ESFX_Fail, "Sound/Fail.wav", 5);

	m_SoundMgr.SetBGMVolume(0.9f); // 볼륨 조절(0.0f ~ 1.0f)
	m_SoundMgr.PlayBGM(EBGM::EBGM_Main);
}

void UManager::ProgressStage()
{
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

void UManager::OnMouseClick()
{
	// 1. 현재 상태에게 클릭 이벤트를 먼저 알립니다.
	if (m_pCurrentState) m_pCurrentState->OnMouseClick(this);

	// 2. 공통 효과음 재생
	m_SoundMgr.PlaySFX(ESFX::ESFX_MouseClick);
}

UManager::UManager(ID3D11Device* device)
	: m_pCurrentState(nullptr), CurStage(EStage::ES_None), CurAvailableStage(EStage::ES_Stage1),
	FileName("ranking.txt"), ResourceManager(nullptr), Score(0.f)
{
	BootGame(device);

	ProbeResource.GenerateTriangle();
	SphereResource.GenerateSphere(1.0f);

	PlanetList.reserve(PlanetListReservedSize);
}

MeshResource* UManager::getProbeResource() 
{	
	return &ProbeResource;
}
MeshResource* UManager::getSphereResource() 
{	
	return &SphereResource;
}

void UManager::setProbeResource( MeshResource& mr)
{
	this->ProbeResource = mr;
}
void UManager::setSphereResource( MeshResource& mr)
{
	this->SphereResource = mr;
}



void UManager::Release()
{
	// 1. 사운드 자원 해제 (DirectSound 인터페이스 및 버퍼 정리)
	m_SoundMgr.Dispose();

	// 2. 게임 데이터 정리 및 저장 (점수 저장 등 기존 로직)
	ShutDownGame();
}


void MeshResource::GenerateTriangle()
{
	Vertices = {
		// Position          // Color (RGBA)
		{  0.0f,  0.3f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f }, // 위 (빨강)
		{  0.3f, -0.3f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f }, // 오른쪽 아래 (초록)
		{ -0.3f, -0.3f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f }  // 왼쪽 아래 (파랑)
	};

	Indexes = { 0, 1, 2 };

	VertexCount = Vertices.size();
	IndexCount = Indexes.size();

}

void MeshResource::GenerateSphere(float radius){
	// 적절한 정밀도 설정 (값이 클수록 매끄럽지만 계산량이 늘어남)
	const uint32_t sliceCount = 20;
	const uint32_t stackCount = 20;

	Vertices.clear();
	Indexes.clear();

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

			Vertices.push_back(v);
		}
	}

	// 2. 인덱스(Index) 생성
	for (uint32_t i = 0; i < stackCount; ++i) {
		for (uint32_t j = 0; j < sliceCount; ++j) {
			uint32_t first = i * (sliceCount + 1) + j;
			uint32_t second = first + sliceCount + 1;

			// 삼각형 1
			Indexes.push_back(second);
			Indexes.push_back(first + 1);
			Indexes.push_back(first);

			// 삼각형 2
			Indexes.push_back(second + 1);
			Indexes.push_back(first + 1);
			Indexes.push_back(second);
		}
	}

	VertexCount = static_cast<unsigned int>(Vertices.size());
	IndexCount = static_cast<unsigned int>(Indexes.size());


}
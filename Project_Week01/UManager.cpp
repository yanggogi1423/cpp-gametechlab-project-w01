#include "UManager.h"
#include "UPrimitive.h"
#include "Probe.h"
#include "USphere.h"
#include "datatype.h"
#include "USoundManager.h"

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

	// 플레이어를 미리 생성하여 nullptr 에러를 방지하고 위치를 고정합니다.
	Player = new Probe();
	Player->SetLocation({ 0.0f, 0.0f, 0.0f });
}

void UManager::InGameReadyInit()
{
	ClearGameObjects();

	CurRunState = ERunstate::ERS_InGameReady;

	// [중요] CurStage에 해당하는 정보를 StageInfoList에서 찾아와서 
	// 장애물이나 플레이어 시작 위치를 설정하는 로직이 여기에 들어와야 합니다.
	// FStageInfo currentInfo = StageInfoList[(int)CurStage - 1];
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

	PlanetList.push_back(*TestPlanet);

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

	//for (auto planet : PlanetList)
	//{
	//	if (planet) delete planet;
	//}

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

	for (auto s : ScoreList)
	{
		ofs << std::get<1>(s) << "," << std::get<1>(s) << "\n";
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

	m_SoundMgr.SetBGMVolume(0.9f); // 볼륨 조절(0.0f ~ 1.0f)
	m_SoundMgr.PlayBGM(EBGM::EBGM_Main);
}

// 마우스 클릭 시 호출될 함수
void UManager::OnMouseClick()
{
	// 세부 재생 로직은 SoundManager가 알아서 합니다.
	m_SoundMgr.PlaySFX(ESFX::ESFX_MouseClick);
}

void UManager::OnPlayClicked()
{
	CurRunState = ERunstate::ERS_StageSelect;
}

void UManager::OnHomeClicked()
{
	MainInit();
}

void UManager::OnSimulationStart()
{
	// 배치 완료 후 시뮬레이션 시작
	InGameRunInit();
}

void UManager::OnRestartClicked()
{
	// 현재 스테이지를 다시 준비 상태로
	InGameReadyInit();
}

void UManager::OnNextStageClicked()
{
	// 다음 스테이지를 자동으로 선택
	EStage nextStage = (EStage)((int)CurStage + 1);
	OnStageSelected(nextStage);
}

// 스테이지 선택 시 호출될 함수
void UManager::OnStageSelected(EStage selected)
{
	if ((int)selected > (int)CurAvailableStage) return;
	
	CurStage = selected;
	LoadingTimer = 2.0f; // 로딩 시간 설정
	CurRunState = ERunstate::ERS_Loading;
}

// 결과 화면 시 호출될 함수
void UManager::OnStageResult(bool bSuccess) {
	CurRunState = ERunstate::ERS_Ending;

	if (bSuccess) {
		// [해금 로직] 현재 스테이지를 깼고, 다음 스테이지가 아직 잠겨있다면 해금
		if (CurStage == CurAvailableStage) {
			if (CurAvailableStage == EStage::ES_Stage1) CurAvailableStage = EStage::ES_Stage2;
			else if (CurAvailableStage == EStage::ES_Stage2) CurAvailableStage = EStage::ES_Stage3;
		}
	}
	// EndingInit(bSuccess, score, name); 호출 등으로 이어짐
}


/* Cons, Des */
UManager::UManager(ID3D11Device* device)
	: CurRunState(ERunstate::ERS_Boot), CurStage(EStage::ES_None), CurAvailableStage(EStage::ES_Stage1),FileName("ranking.txt"),ResourceManager(nullptr),Score(0.f)
	//,bBootDone(false), bIsAlreadyDestroy(false)
{
	BootGame(device);

	// vertex init
	ProbeResource.GenerateTriangle();
	SphereResource.GenerateSphere();

	// planet list
	// usphere 초기화를 해야하는데....
	PlanetList = std::vector<USphere>(50);

}

void UManager::Update(float deltaTime)
{
	switch (CurRunState)
	{
	case ERunstate::ERS_Main:
		break;
	case ERunstate::ERS_Loading:
		LoadingTimer -= deltaTime;
		if (LoadingTimer <= 0.0f)
		{
			InGameReadyInit();
		}
		break;

	case ERunstate::ERS_InGameReady:
		break;
	case ERunstate::ERS_InGameRun:

		//	Value Input
		ComputePhysicsAndApply(deltaTime);

		//	Physics Update
		Player->SetLocation(Player->GetLocation() + Player->GetVelocity() * deltaTime);

		//  Renderer에서 Player의 Location과 PlanetList의 Location을 참조하여 그려줌
		CollisionDetection();	//	필요하다면 반복
		break;
	case ERunstate::ERS_Ending:
		break;
	default:
		break;
	}

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
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

void UManager::MainInit()
{
	//ClearGameObjects();
	CurRunState = ERunstate::ERS_Main;

	// 테스트용 행성 생성 (나중에 제거)
	Player = new Probe();
	Player->SetLocation({ 0.0f, 0.0f, 0.0f });
	Player->SetScale(0.1f);

	PlanetList.emplace_back();
	USphere& testP = PlanetList.back();
	testP.SetLocation({ 0.5f, 0.0f, 0.0f });
	testP.SetScale(0.1f);
}

void UManager::StageSelectInit()
{
	//ClearGameObjects();
	
	CurRunState = ERunstate::ERS_StageSelect;
}

void UManager::InGameReadyInit()
{
	//ClearGameObjects();

	CurRunState = ERunstate::ERS_InGameReady;

	int StageIdx = (int)CurStage - 1;
	RemainTimer = StageInfoList[StageIdx].MaxTime;

	if (StageIdx < 0 || StageIdx >= StageInfoList.size()) return;

	FStageInfo StageInfo = StageInfoList[StageIdx];

	for (const auto& obstacle : StageInfo.ObstacleList)
	{
		// new를 쓰지 않고 emplace_back을 사용하여 벡터 내부에 직접 객체를 생성합니다.
		// 이렇게 하면 메모리 관리를 벡터가 100% 책임집니다.
		PlanetList.emplace_back();

		// 방금 추가된 마지막 객체에 접근하여 데이터를 설정합니다.
		USphere& planet = PlanetList.back();
		planet.SetLocation(obstacle.second);
		planet.SetMass(10.0f);
		planet.SetScale(0.1f);
	}

	// 플레이어(Probe)는 단일 객체이므로 기존 포인터 방식을 유지하거나 
	// 원하신다면 이 역시 객체로 바꿀 수 있습니다.
	if (Player == nullptr) Player = new Probe();
	Player->SetLocation({ 0.0f, -0.8f, 0.0f });
}

void UManager::InGameRunInit()
{
	CurRunState = ERunstate::ERS_InGameRun;
}

//	Stage를 클리어하거나, 플레이어가 사망했을 때 호출
//	bIsClear는 클리어 여부를 담음. (main에서 호출됨) -> default parameter를 고려하여 설계
//	name은 사용자에게 직접 입력 받습니다.
//void UManager::EndingInit(bool bIsClear, unsigned int score, std::string name)
//{
//	CurRunState = ERunstate::ERS_Ending;
//
//	ClearGameObjects();
//
//	if (bIsClear)
//	{
//		m_SoundMgr.PlaySFX(ESFX::ESFX_Clear); 
//	}
//	else
//	{
//		m_SoundMgr.PlaySFX(ESFX::ESFX_Fail);
//	}
//
//	
//
//	//	Score Display
//	DisplayScore(name, score);
//}

//	InGameReady 상태로 분기 시 플레이어 및 장애물 생성
void UManager::InitGameObjects()
{
	// 1. 플레이어 생성 (Player가 포인터 유지라면 new 사용, 객체라면 emplace_back 고려)
	if (Player == nullptr) Player = new Probe();
	Player->SetLocation({ 0.0f, -0.8f, 0.0f });
	Player->SetVelocity({ 0.0f, 0.0f, 0.0f }); // 시작 시 속도 초기화

	// 2. 테스트 행성 생성 (emplace_back 활용)
	// 임시 포인터를 만들지 않고 벡터 내부에 직접 공간을 할당합니다.
	PlanetList.emplace_back();

	// 3. 방금 생성된 마지막 객체(TestPlanet) 설정
	USphere& testPlanet = PlanetList.back();
	testPlanet.SetLocation({ 0.5f, 0.8f, 0.0f });
	testPlanet.SetVelocity({ 0.0f, -0.2f, 0.0f });
	testPlanet.SetScale(0.1f); // 크기가 너무 크면 여기서 조절
	testPlanet.SetMass(10.0f);

	// 이제 PlanetList는 객체 리스트이므로 
	// 이 함수가 끝나도 행성 데이터는 벡터 안에 안전하게 보존됩니다.
}

//	모든 오브젝트 삭제
//	RunState와 Stage 상태 변동시 항상 적용
//void UManager::ClearGameObjects()
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

void UManager::BootGame(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	if (CurRunState != ERunstate::ERS_Boot)
	{
		//	TODO : Make an error log
		return;
	}
	
	//	1. Local Score 및 ResourceManager 로드
	//LoadScore(); //-> EndingState로 이동
	ResourceManager = new UResourceManager();
	ResourceManager->Initialize(device, deviceContext);

	InputManager = new PlayerInput();

	//	2. 스테이지 정보 생성
	StageInfoList.push_back({ EStage::ES_Stage1,30.f });
	StageInfoList.push_back({ EStage::ES_Stage2,30.f });
	StageInfoList.push_back({ EStage::ES_Stage3,30.f });

	//	3. 메인 State로 분기
	MainInit();

}

//	외부에서 호출해줘야 함 (혹은 UManager Destructor에서 호출됨)
// EndingState로 이동
//void UManager::ShutDownGame()
//{
//	if (CurRunState == ERunstate::ERS_Destroy) 
//	{
//		//	TODO : Make an error log
//		return;
//	}
//
//	SaveScore();
//
//
//
//	//	1. Heap 해제
//	ResourceManager->Release();
//
//	CurRunState = ERunstate::ERS_Destroy;
//}

/* Non-game Management */
//	DataStruct : <stage>,<nickname>,<score> (CSV-like based txt)
//	실행 시에만 Load
// EndingState로 이동
//void UManager::LoadScore()
//{
//	ScoreList.clear();
//
//	std::ifstream ifs(FileName);
//	std::string line;
//
//
//	while (std::getline(ifs, line))
//	{
//		std::stringstream ss(line);
//
//		unsigned int stage;
//		std::string name;
//		unsigned int score;
//
//		char dummy;
//
//		//	구분자 : ,
//		ss >> stage;
//		ss >> dummy;	//	첫 번째 ',' 가짐
//		std::getline(ss, name, ',');
//		ss >> score;
//
//		ScoreList.push_back({ stage, name, score });
//	}
//
//	ifs.close();
//}

//	Vector 정렬 후 File 형식에 맞추어 Parsing
//	이는 Shutdown
// 	EndingState로 이동
//void UManager::SaveScore()
//{
//	std::ofstream ofs(FileName);
//	// ScoreList 튜플 구조 <Stage, Name, Score>에 맞춰 저장합니다.
//	for (const auto& s : ScoreList)
//	{
//		ofs << std::get<0>(s) << "," << std::get<1>(s) << "," << std::get<2>(s) << "\n";
//	}
//	ofs.close();
//}

//	File이 아닌 Runtime Vector를 통해 읽어옴
// 
//EndingState로 이동
//void UManager::DisplayScore(std::string name, unsigned int score)
//{
//	int stage = -1;
//	switch (CurStage)
//	{
//	case EStage::ES_Stage1:
//		stage = 1;
//		break;
//	case EStage::ES_Stage2:
//		stage = 2;
//		break;
//	case EStage::ES_Stage3:
//		stage = 3;
//		break;
//	default:
//		break;
//	}
//	//	List에 포함해서 보여주기 (포함 후 정렬 -> 보여주기)
//	ScoreList.push_back({ stage, name, score });
//	std::sort(ScoreList.begin(), ScoreList.end(), [](const auto& a, const auto& b)
//		{
//			//	다른 스테이지일 경우 Sort하지 않음
//			//	Display 시에도 Stage로 filtering하면 됨
//			if (std::get<0>(a) != std::get<0>(b)) return false;
//
//			return std::get<2>(a) < std::get<2>(b);
//		}
//	);
//
//	//	TODO : Imgui를 통해 display
//}


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

//EndingState
//void UManager::ProgressStage()
//{
//	switch (CurAvailableStage)
//	{
//	case EStage::ES_Stage1:
//		CurAvailableStage = EStage::ES_Stage2;
//		break;
//
//	case EStage::ES_Stage2:
//		CurAvailableStage = EStage::ES_Stage3;
//		break;
//
//	case EStage::ES_Stage3:
//		//	Do nothing
//		break;
//	}
//
//	ClearGameObjects();
//}

// 마우스 클릭 시 호출될 함수
void UManager::OnMouseClick()
{
	// 세부 재생 로직은 SoundManager가 알아서 합니다.
	m_SoundMgr.PlaySFX(ESFX::ESFX_MouseClick);
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
// EndingState로 이동 
//void UManager::OnStageResult(bool bSuccess) 
//{
//	ProgressStage();
//
//	unsigned int finalScore = 0;
//	if (bSuccess) finalScore = (unsigned int)(RemainTimer * RemainTimer);
//	else finalScore = 0;
//
//	EndingInit(bSuccess, finalScore, RandomNameGenerator());
//}


/* Cons, Des */

UManager::UManager(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: CurRunState(ERunstate::ERS_Boot), CurStage(EStage::ES_None), CurAvailableStage(EStage::ES_Stage1), ResourceManager(nullptr), Score(0.f)
{
	BootGame(device , deviceContext);

	// 정점/인덱스 데이터 시트 생성 (실제 버퍼는 main의 createBuffer에서 생성됨)
	ProbeResource.GenerateTriangle();
	SphereResource.GenerateSphere(1.0f);

	PlanetList.reserve(PlanetListReservedSize);
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
		// Remaining Time Update
		RemainTimer -= deltaTime;
		if (RemainTimer <= 0.0f)
		{
			//OnStageResult(false); // 시간 초과 실패
			return;
		}
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
	//ShutDownGame();
}


void MeshResource::GenerateTriangle()
{
	// FTextureVertex 구조: { x, y, z,  r, g, b, a,  u, v }
	Vertices = {
		// 위쪽 꼭짓점: 텍스처의 상단 중앙 (0.5, 0.0)
		{  0.0f,  0.3f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.5f, 0.0f },

		// 오른쪽 아래: 텍스처의 우측 하단 (1.0, 1.0)
		{  0.3f, -0.3f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f },

		// 왼쪽 아래: 텍스처의 좌측 하단 (0.0, 1.0)
		{ -0.3f, -0.3f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f }
	};

	// 시계 방향(CW)으로 인덱스 설정
	Indexes = { 0, 1, 2 };

	VertexCount = static_cast<UINT>(Vertices.size());
	IndexCount = static_cast<UINT>(Indexes.size());
}

void MeshResource::GenerateSphere(float radius)
{
	// 원을 구성할 삼각형의 개수 (높을수록 부드러워짐)
	const uint32_t segmentCount = 50;

	Vertices.clear();
	Indexes.clear();

	// 1. 중심 정점 생성 (UV: 0.5, 0.5)
	FTextureVertex center;
	center.x = 0.0f; center.y = 0.0f; center.z = 0.0f;
	center.r = 1.0f; center.g = 1.0f; center.b = 1.0f; center.a = 1.0f;
	center.u = 0.5f; center.v = 0.5f;
	Vertices.push_back(center);

	// 2. 테두리 정점 생성
	for (uint32_t i = 0; i <= segmentCount; ++i)
	{
		float theta = 2.0f * 3.1415926535f * (float)i / segmentCount;

		float cosT = cosf(theta);
		float sinT = sinf(theta);

		FTextureVertex v;
		v.x = radius * cosT;
		v.y = radius * sinT;
		v.z = 0.0f;

		v.r = 1.0f; v.g = 1.0f; v.b = 1.0f; v.a = 1.0f;

		// UV 좌표: 중심(0.5, 0.5)을 기준으로 -1~1 범위를 0~1로 변환
		// cosT는 -1~1이므로 0.5를 곱하고 0.5를 더함
		v.u = cosT * 0.5f + 0.5f;
		v.v = 1.0f - (sinT * 0.5f + 0.5f); // DirectContext 좌표계에 맞게 Y(V) 반전

		Vertices.push_back(v);
	}

	// 3. 인덱스 생성 (중심점을 포함한 삼각형 리스트)
	for (uint32_t i = 1; i <= segmentCount; ++i)
	{
		Indexes.push_back(0);         // 중심점
		Indexes.push_back(i + 1);     // 다음 테두리 점
		Indexes.push_back(i);         // 현재 테두리 점
	}

	VertexCount = static_cast<UINT>(Vertices.size());
	IndexCount = static_cast<UINT>(Indexes.size());
}
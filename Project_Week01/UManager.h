#pragma once

#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <random>


#include "Probe.h"
#include "datatype.h"
#include "USoundManager.h"
#include "UPrimitive.h"
#include "USphere.h"


//	Constants
constexpr float TopBorder = -1.f;
constexpr float BottomBorder = 1.f;
constexpr float LeftBorder = -1.f;
constexpr float RightBorder = 1.f;

constexpr size_t PlanetListReservedSize = 50;
constexpr float GravititationalConstant = 9.8f;

#pragma region __GAME_STATE__

//	None 상태를 갖지 않음 (MECE)
enum class ERunstate
{
	ERS_Boot,
	ERS_Main,
	ERS_StageSelect,
	ERS_InGameReady,
	ERS_InGameRun,
	ERS_Ending,	//	플레이어의 사망 || 게임 클리어
	ERS_Destroy	//	Destroy process가 끝나면 실행됨
};

#pragma endregion

#pragma region __STAGE_INFO__

enum class EStage
{
	ES_None,
	ES_Stage1,
	ES_Stage2,
	ES_Stage3,
};

//	스테이지에 대한 정보 저장
struct FStageInfo
{
	EStage StageName;
	float MaxTime;	//	Seconds

	//	1. 행성의 종류(idx 구분)와 개수에 대한 정보
	std::vector<std::pair<int, int>> CreatablePlanetList; //인덱스와 개수 정보

	//	2. 장애물 위치 및 종류(idx), 개수 정보 -> 위치, 개수를 pair (혹은 개수만)
	std::vector<std::pair<int, FVector>> ObstacleList; //인덱스와 위치 정보

	//	3. 탈출구 객체 위치
	FVector ExitLocation;
};

#pragma endregion


struct ID3D11Buffer;

class UManager
{
private:
	//	Game State
	ERunstate CurRunState;
	EStage CurStage;			//	현재 선택한 스테이지
	EStage CurAvailableStage;	//	선택 가능한 스테이지

	//	Time
	float RemainTimer;

	//	Boot and Destroy - 반복 호출을 막기 위한 flag -> Flag 말고 Enum으로 변경
	//bool bBootDone;
	//bool bIsAlreadyDestroy;

private:
	const std::string FileName;
	std::vector<std::pair<std::string, unsigned int>> ScoreList;

	/* GameObjects */
	Probe* Player;
	std::vector<USphere*> PlanetList;	//	이후에 template 수정할 수도 있음

	/* Game Data */
	std::vector<FStageInfo> StageInfoList;
	
	/* Other Managers */
	USoundManager* SoundManager;

	/* Game Management */
private:
	//	Collision 관련
	void CollisionDetection();
	void CollsionResolution();

	//	Scene Initialization
	void MainInit();		//	Opening(시작 화면)으로 분기
	void InGameReadyInit();	//	행성 배치 가능 상태로 분기
	void InGameRunInit();	//	사실상 Simulation Start
	void EndingInit(bool bIsClear, unsigned int score, std::string name = RandomNameGenerator());

	//	Stage Progress
	void ProgressStage();
	void InitGameObjects();
	void ClearGameObjects();

	//	Game Logic
	//	행성 배치

	void ComputePhysicsAndApply(float deltaTime);


	/* Non-game Management */
	void BootGame();	//	Application 실행 시 호출 (게임 데이터 준비)
	void ShutDownGame();	//	Application 종료 시 호출 (게임 데이터 정리 및 저장)

	//	File Load
	void LoadScore();
	void SaveScore();
	void DisplayScore(std::string name, unsigned int score);
	



public:
	//	Ranking System에서 유저 이름을 등록하지 않으면 Random String으로 지정함.
	static std::string RandomNameGenerator()
	{
		const std::string charSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
		const int appendLength = 8;

		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<int> dist(0, charSet.size() - 1);

		std::string name = "User_";

		for (int i = 0; i < appendLength; i++)
		{
			name += charSet[i];
		}

		return name;
	}

	/* Cons, Des */
	UManager()
		: CurRunState(ERunstate::ERS_Boot), 
		CurStage(EStage::ES_None), CurAvailableStage(EStage::ES_Stage1),
		FileName("ranking.txt"),
		SoundManager(nullptr)
		//,bBootDone(false), bIsAlreadyDestroy(false)
	{
		BootGame();
	}
	~UManager()
	{
		ShutDownGame();
	}

	void Update(float deltaTime);


	/* Getter, Setter */
	const Probe& GetProbe() const { return (*Player);  }
	const std::vector<USphere *> & GetPlanetList() const { return PlanetList; }

	bool Startable() const { return CurRunState != ERunstate::ERS_Boot; }

	// mesh info
private:
	struct MeshResource {
		ID3D11Buffer* VB = nullptr;      // Vertex Buffer
		ID3D11Buffer* IB = nullptr;      // Index Buffer 
		unsigned int VertexCount = 0;    // 정점 개수
		unsigned int IndexCount = 0;     // 인덱스 개수 (Draw 호출 시 필수)
		unsigned int Stride = 0;         // 정점 1개의 크기 (sizeof(FVertex))

		void initResource(ID3D11Buffer* vb, ID3D11Buffer* ib, unsigned int vertexCount, unsigned int indexCount, unsigned int stride) {
			VB = vb;
			IB = ib;
			VertexCount = vertexCount;
			IndexCount = indexCount;
			Stride = stride;
		}

		ID3D11Buffer* getVB() const
		{
			return VB;
		}
		ID3D11Buffer* getIB() const
		{
			return IB;
		}
		const unsigned int getVertexCount()
		{
			return VertexCount;
		}
		const unsigned int getIndexCount()
		{
			return IndexCount;
		}
		const unsigned int getStride()
		{
			return Stride;
		}


	}; 

	MeshResource ProbeResource;
	MeshResource SphereResource;
public:

	void initResource(RESOURCE_TYPE rt,ID3D11Buffer* vb, ID3D11Buffer* ib, unsigned int vertexCount, unsigned int indexCount, unsigned int stride);
	

};
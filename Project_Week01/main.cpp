#include "URenderer.h"
#include "UManager.h"
#include "Probe.h"

#include <windows.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#include "UImanager.h"


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static UManager* g_Manager = nullptr; // 전역 UManager 포인터

// 각종 메시지를 처리할 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//버튼 입력 받기 준비
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
		// Signal that the app should quit
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

struct FVertexStruct;

// 나중에 랜덤 생성
static DirectX::XMFLOAT3 myPos = { -1.0f, -1.0f, 0.0f };
static DirectX::XMMATRIX matScale;
//manager
inline void updateConstant(URenderer * renderer )
{

	using namespace DirectX;


	// TODO manager의 값 받아서 x,  y , z update
	FVector vel(0.1f, 0.1f, 0.0f);

	myPos.x += vel.x;
	myPos.y += vel.y;
	matScale = XMMatrixScaling(0.1f, 0.1f, 0.1f);

	// mypos 바꿔야함
	XMMATRIX matTranslate = XMMatrixTranslation(myPos.x, myPos.y, myPos.z);
	
	XMMATRIX constant = matScale * matTranslate;
	constant = XMMatrixTranspose(constant);

	renderer->UpdateConstant(constant);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// 윈도우 클래스 이름
	WCHAR WindowClass[] = L"JungleWindowClass";

	// 윈도우 타이틀바에 표시될 이름
	WCHAR Title[] = L"Team3_week1";

	// 각종 메시지를 처리할 함수인 WndProc의 함수 포인터를 WindowClass 구조체에 넣는다.
	WNDCLASSW wndclass = { 0, WndProc, 0, 0, 0, 0, 0, 0, 0, WindowClass };

	// 윈도우 클래스 등록
	RegisterClassW(&wndclass);

	// 1024 x 1024 크기에 윈도우 생성
	HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024,
		nullptr, nullptr, hInstance, nullptr);

	bool bIsExit = false;

	// 각종 생성하는 코드를 여기에 추가합니다.

	UManager* manager = new UManager();
	g_Manager = manager;	

	manager->Initialize(hWnd);

	URenderer* renderer = new URenderer();
	Probe probe;
	renderer->Create(hWnd);
	renderer->CreateShader();

	
	// constant 만들기
	renderer->CreateConstantBuffer();


	/* UI Initialization */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init((void*)hWnd);
	ImGui_ImplDX11_Init(renderer->Device, renderer->DeviceContext);


	// [추가] 고정밀 타이머 변수 초기화
	LARGE_INTEGER freq, prevTime, currTime;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&prevTime);
	float deltaTime = 0.0f;

	// Main Loop
	while (bIsExit == false)
	{
		MSG msg;

		// 메시지 처리 루프
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				bIsExit = true;
				break;
			}
		}

		if (bIsExit) break;

		// DeltaTime 계산 (프레임 간 경과 시간)
		QueryPerformanceCounter(&currTime);
		deltaTime = static_cast<float>(currTime.QuadPart - prevTime.QuadPart) / static_cast<float>(freq.QuadPart);
		prevTime = currTime;

		////////////////////////////////////////////
		// 매번 실행되는 코드를 여기에 추가합니다.

		renderer->Prepare();
		renderer->PrepareShader();

		// 3. [추가] 매니저 업데이트 (물리 계산, 플레이어 이동, 충돌 감지)
		// 이 안에서 deltaTime을 사용하여 일정한 속도로 움직이게 됩니다.
		manager->Update(deltaTime);

		// 4. 상수 버퍼 업데이트 및 렌더링
		updateConstant(renderer);
		renderer->RenderPrimitive(triangle); // 렌더링 호출 확인

		renderer->SwapBuffer();
		////////////////////////////////////////////
	}

	manager->Release();
	delete manager;

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

	renderer->ReleaseShader();
	renderer->Release();

	delete renderer;
	return 0;
}

#include "URenderer.h"
#include "UManager.h"
#include "Probe.h"

#include <windows.h>

// 각종 메시지를 처리할 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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

	URenderer* renderer = new URenderer();
	Probe probe;
	renderer->Create(hWnd);
	renderer->CreateShader();
	
	


	
	// constant 만들기
	renderer->CreateConstantBuffer();




	// Main Loop (Quit Message가 들어오기 전까지 아래 Loop를 무한히 실행하게 됨)
	while (bIsExit == false)
	{
		MSG msg;

		// 처리할 메시지가 더 이상 없을때 까지 수행
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// 키 입력 메시지를 번역
			TranslateMessage(&msg);

			// 메시지를 적절한 윈도우 프로시저에 전달, 메시지가 위에서 등록한 WndProc 으로 전달됨
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				bIsExit = true;
				break;
			}
		}

		////////////////////////////////////////////
		// 매번 실행되는 코드를 여기에 추가합니다.

		renderer->Prepare();
		renderer->PrepareShader();

		updateConstant(renderer);
			
		renderer->SwapBuffer();
		////////////////////////////////////////////
	}

	// 소멸하는 코드를 여기에 추가합니다.

	renderer->ReleaseShader();
	renderer->Release();

	delete renderer;
	return 0;
}

#include "URenderer.h"
#include "UManager.h"

#include <windows.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#include "UImanager.h"
#include "UResourceManager.h"

#include "ExampleStateManager.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


static UManager* g_Manager = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_LBUTTONDOWN:
		if (g_Manager) g_Manager->OnMouseClick();
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

static DirectX::XMFLOAT3 myPos = { 0.0f, 0.0f, 0.0f };
static DirectX::XMMATRIX matScale;

inline void updateConstant(URenderer* renderer, float deltaTime)
{
	using namespace DirectX;

	myPos.x += 0.1f * deltaTime;
	matScale = XMMatrixScaling(0.1f, 0.1f, 0.1f);

	XMMATRIX matTranslate = XMMatrixTranslation(myPos.x, myPos.y, myPos.z);
	XMMATRIX constant = matScale * matTranslate;
	constant = XMMatrixTranspose(constant);

	renderer->UpdateConstant(constant);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WCHAR WindowClass[] = L"JungleWindowClass";
	WCHAR Title[] = L"Team3_week1";

	WNDCLASSW wndclass = { 0, WndProc, 0, 0, 0, 0, 0, 0, 0, WindowClass };
	RegisterClassW(&wndclass);

	HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024,
		nullptr, nullptr, hInstance, nullptr);

	// 1. 매니저 및 렌더러 생성

	URenderer* renderer = new URenderer();
	renderer->Create(hWnd);
	renderer->CreateShader();
	renderer->CreateConstantBuffer();

	//////////////////////1.Imgui 초기화//////////////////////
	UIManager::InitImGui(hWnd, renderer);
	///////////////////////////////////////////////////////


	ExampleStateManager temp = ExampleStateManager();

	UManager* manager = new UManager(renderer->Device);
	g_Manager = manager;
	manager->Initialize(hWnd); // 사운드 여기서 시작!

	// 2. GPU 버퍼 생성 및 Manager 등록
	std::vector<FVertex> triVertices;
	std::vector<unsigned int> triIndices;
	GenerateVertices::GenerateTriangle(triVertices, triIndices);

	ID3D11Buffer* vBuffer = nullptr;
	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(FVertex) * (UINT)triVertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA vinitData = { triVertices.data() };
	renderer->Device->CreateBuffer(&vbd, &vinitData, &vBuffer);

	// Manager에게 이 버퍼를 쓰라고 등록합니다.
	manager->initResource(PROBE, vBuffer, nullptr, (UINT)triVertices.size(), 0, sizeof(FVertex), 1.0f);

	
	// 타이머 설정
	LARGE_INTEGER freq, prevTime, currTime;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&prevTime);
	float deltaTime = 0.0f;

	bool bIsExit = false;
	while (!bIsExit)
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) bIsExit = true;
		}
		if (bIsExit) break;

		QueryPerformanceCounter(&currTime);
		deltaTime = static_cast<float>(currTime.QuadPart - prevTime.QuadPart) / static_cast<float>(freq.QuadPart);
		prevTime = currTime;

		// --- Rendering ---
		renderer->Prepare();

		renderer->PrepareShader();
		manager->Update(deltaTime);
		updateConstant(renderer, deltaTime);

		// Manager로부터 버퍼를 받아와 렌더링
		MeshResource probeRes = manager->getProbeResource();
		if (probeRes.VB != nullptr)
		{
			renderer->RenderPrimitive(probeRes.VB, probeRes.VertexCount);
		}

		//////////////////////////uiManager.Render() 호출///////////////////////
		//uiManager를 여러개를 동신에 Render하면 안됩니다. 여러 프레임을 그리고 싶다면 uiManager 하나를 만들고 그 안에서 여러 프레임을 만들어야 합니다.
		//꼭 uiManager만 고집할 필요는 없고 다른 씬을 구성하는 클래스의 멤버로 uiManager를 만들어서 호출해도 됩니다.
		temp.Update(renderer);
		////////////////////////////////////////////////////////////////////////

		renderer->SwapBuffer();
	}


	manager->Release(); // 사운드 해제 포함
	delete manager;

	/////////////////////////ui.Maanger 종료////////////////////////
	//종료시 imGui를 해제합니다.
	UIManager::ShutdownImGui();
	////////////////////////////////////////////////////////////////

	if (vBuffer) vBuffer->Release();
	renderer->Release();
	delete renderer;

	return 0;
}
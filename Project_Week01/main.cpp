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

inline void createBuffer(UManager* manager, URenderer* renderer)
{
	MeshResource* probe = manager->getProbeResource();
	MeshResource* sphere = manager->getSphereResource();

	renderer->CreateVertexBuffer(probe->VB, probe->Vertices.data(), probe->Vertices.size() * sizeof(FTextureVertex));
	renderer->CreateVertexBuffer(sphere->VB, sphere->Vertices.data(), sphere->Vertices.size() * sizeof(FTextureVertex));

	renderer->CreateIndexBuffer(probe->IB, probe->Indexes.data(), probe->IndexCount);
	renderer->CreateIndexBuffer(sphere->IB, sphere->Indexes.data(), sphere->IndexCount);

	manager->setProbeResource(*probe);
	manager->setSphereResource(*sphere);
}



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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WCHAR WindowClass[] = L"JungleWindowClass";
	WCHAR Title[] = L"Team3_week1";

	WNDCLASSW wndclass = { 0, WndProc, 0, 0, 0, 0, 0, 0, 0, WindowClass };
	RegisterClassW(&wndclass);

	//	4 * 3 (1400 * 1050)
	HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, WindowWidth, WindowHeight,
		nullptr, nullptr, hInstance, nullptr);

	// 1. 매니저 및 렌더러 생성

	URenderer* renderer = new URenderer();
	renderer->Create(hWnd);
	renderer->CreateShader();
	renderer->CreateConstantBuffer();
	//////////////////////1.Imgui 초기화//////////////////////
	UIManager::InitImGui(hWnd, renderer);
	///////////////////////////////////////////////////////



	UManager* manager = new UManager(renderer->Device , renderer->DeviceContext);

	g_Manager = manager;
	manager->Initialize(hWnd); // 사운드 여기서 시작!
	
	createBuffer(manager, renderer);

	ExampleStateManager temp = ExampleStateManager(manager);

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

		// ImGui 프레임 시작 신호 (없으면 프리징 발생!)
		//ImGui_ImplDX11_NewFrame();
		//ImGui_ImplWin32_NewFrame();
		//ImGui::NewFrame();

		renderer->PrepareShader();
		manager->Update(deltaTime);

		// 1. 플레이어(Probe) 렌더링
		Probe* pPlayer = manager->GetProbe();
		if (pPlayer != nullptr)
		{
			// 객체 스스로 계산한 행렬을 렌더러의 상수 버퍼에 직접 전송합니다.
			renderer->UpdateConstant(pPlayer->GetTransformMatrix());

			MeshResource* probeRes = manager->getProbeResource();
			if (probeRes->VB != nullptr)
			{
				renderer->indexRenderPrimitive(probeRes->VB ,probeRes->IB,probeRes->IndexCount);
				renderer->textureRenderPrimitive(probeRes->VB, probeRes->IB, probeRes->IndexCount, manager->GetResourceManager()->GetTexture(ImageName::ROCKET ));
			}
		}

		// 2. 행성(Sphere)들 렌더링 (추후 확장을 위해)
		for (auto& planet : manager->GetPlanetList())
		{
			// 각 행성도 자신만의 Scale과 Location이 담긴 행렬을 보냅니다.
			renderer->UpdateConstant(planet.GetTransformMatrix());

			MeshResource* sphereRes = manager->getSphereResource();
			if (sphereRes->VB != nullptr)
			{
				renderer->indexRenderPrimitive(sphereRes->VB, sphereRes->IB, sphereRes->IndexCount);
			}
		}

		temp.Update(renderer);

		renderer->SwapBuffer();
	}

	
	manager->Release(); // 사운드 해제 포함
	delete manager;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	
	renderer->Release();
	delete renderer;

	return 0;
}
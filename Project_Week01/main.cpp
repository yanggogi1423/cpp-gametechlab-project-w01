#include "URenderer.h"
#include "UManager.h"

#include <windows.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#include "UImanager.h"
#include "UResourceManager.h"

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
	UManager* manager = new UManager();
	g_Manager = manager;
	manager->Initialize(hWnd); // 사운드 여기서 시작!

	URenderer* renderer = new URenderer();
	renderer->Create(hWnd);
	renderer->CreateShader();
	renderer->CreateConstantBuffer();

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

	// UI 초기화
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init((void*)hWnd);
	ImGui_ImplDX11_Init(renderer->Device, renderer->DeviceContext);

	UResourceManager resourceManager;
	resourceManager.Initialize(renderer->Device);

	UIManager uiManager;
	UIFrame& testFrame = uiManager.CreateFrame("Test Frame")
		.Position(ImVec2(10.f, 30.f))
		.Size(ImVec2(300.f, 200.f));

	testFrame.AddText("Hello, ImGui!", ImVec2(10.f, 30.f), resourceManager.FontDefault);
	testFrame.AddImage(resourceManager.SRVBackground, ImVec2(10.f, 40.f), ImVec2(100.f, 100.f));

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
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		renderer->PrepareShader();
		manager->Update(deltaTime);
		updateConstant(renderer, deltaTime);

		// Manager로부터 버퍼를 받아와 렌더링
		MeshResource probeRes = manager->getProbeResource();
		if (probeRes.VB != nullptr)
		{
			renderer->RenderPrimitive(probeRes.VB, probeRes.VertexCount);
		}
		ImGui::Begin("title.c_str(), nullptr, flags");

		ImGui::Image((ImTextureID)resourceManager.SRVBackground, ImVec2(100.f, 100.f));
		ImGui::End();

		// ImGui 실제 렌더링
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		//uiManager.Render();

		renderer->SwapBuffer();
	}

	
	manager->Release(); // 사운드 해제 포함
	delete manager;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (vBuffer) vBuffer->Release();
	renderer->Release();
	delete renderer;

	return 0;
}
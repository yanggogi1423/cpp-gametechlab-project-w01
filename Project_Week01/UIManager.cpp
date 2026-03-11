#include "UIManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

UIFrame& UIManager::CreateFrame(const std::string& title)
{
	UIFrameList.push_back(std::make_unique<UIFrame>(title));
	return *UIFrameList.back();
}

void UIManager::InitImGui(HWND hWnd, URenderer* renderer)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init((void*)hWnd);
	ImGui_ImplDX11_Init(renderer->Device, renderer->DeviceContext);
}

void UIManager::ShutdownImGui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void UIManager::Render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	for (auto& frame : UIFrameList)
	{
		frame->Render();
	}
	 
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
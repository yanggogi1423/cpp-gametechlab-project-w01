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
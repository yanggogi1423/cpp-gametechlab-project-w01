#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include <vector>
#include <functional>
#include <string>
#include "datatype.h"

struct ButtonInfo
{
	std::string label;
	ImVec2 position;
	ImVec2 size;
	std::function<void()> callback;
};

class UIFrame
{
private:
	std::string title;
	ImVec2 position;
	ImVec2 size;
	std::vector<ButtonInfo> buttons;

public:
	UIFrame(const std::string& title, ImVec2 position, ImVec2 size);
	~UIFrame();

	void AddButton(const std::string& label, const ImVec2& position, const ImVec2& size, std::function<void()> callback);
	void Render();

};
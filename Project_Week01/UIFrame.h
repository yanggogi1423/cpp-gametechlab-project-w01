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

struct TextInfo
{
	std::string text;
	ImVec2 position;
	ImFont* font;
};

class UIFrame
{
private:
	std::string title;
	ImVec2 position;
	ImVec2 size;
	ImVec4 backgroundColor;
	float borderLineTransparency;
	bool noTitleBar;

	std::vector<ButtonInfo> buttons;
	std::vector<TextInfo> texts;


public:
	UIFrame(const std::string& title, ImVec2 position, ImVec2 size, bool noTitleBar = false, ImVec4 backgroundColor = ImVec4(39.f / 255.f, 35.f / 255.f, 75.f / 255.f, 1.0f));
	~UIFrame();

	void AddButton(const std::string& label, const ImVec2& position, const ImVec2& size, std::function<void()> callback);
	void AddText(const std::string& text, const ImVec2& position, ImFont* font);
	void SetBorderLineTransparency(float transparency);
	ImVec2 GetPosition() const;
	void SetPosition(const ImVec2& newPosition);
	void Render();

};
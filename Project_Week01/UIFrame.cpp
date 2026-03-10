#include "UIFrame.h"

UIFrame::UIFrame(const std::string& title, ImVec2 position, ImVec2 size) : title(title), position(position), size(size)
{
}

UIFrame::~UIFrame()
{
}

void UIFrame::AddButton(const std::string& label, const ImVec2& position, const ImVec2& size, std::function<void()> callback)
{
	ButtonInfo button;
	button.label = label;
	button.position = position;
	button.size = size;
	button.callback = callback;
	buttons.push_back(button);
}

void UIFrame::AddText(const std::string& text, const ImVec2& position, ImFont* font)
{
	TextInfo textInfo;
	textInfo.text = text;
	textInfo.position = position;
	textInfo.font = font;
	texts.push_back(textInfo);
}

void UIFrame::Render()
{
	ImGui::SetNextWindowPos(position, ImGuiCond_Once);
	ImGui::SetNextWindowSize(size, ImGuiCond_Once);
	ImGui::Begin(title.c_str());

	for (const auto& button : buttons)
	{
		ImGui::SetCursorPos(button.position);

		if (ImGui::Button(button.label.c_str(), button.size))
		{
			button.callback();
		}
	}

	for (const auto& text : texts)
	{
		ImGui::SetCursorPos(text.position);
		ImGui::PushFont(text.font);
		ImGui::Text("%s", text.text.c_str());
		ImGui::PopFont();
	}

	ImGui::End();
}
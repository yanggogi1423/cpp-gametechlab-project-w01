#include "UIFrame.h"

UIFrame::UIFrame(const std::string& title) : title(title)
{
	position = ImVec2(0.0f, 0.0f);
	size = ImVec2(200.0f, 100.0f);
	backgroundColor = ImVec4(39.f / 255.f, 35.f / 255.f, 75.f / 255.f, 1.0f);
	borderLineTransparency = 1.0f;
	noTitleBar = false;
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

void UIFrame::AddImage(ID3D11ShaderResourceView* texture, const ImVec2& position, const ImVec2& size)
{
	ImageInfo imageInfo;
	imageInfo.texture = texture;
	imageInfo.position = position;
	imageInfo.size = size;
	images.push_back(imageInfo);
}

void UIFrame::AddImageButton(ID3D11ShaderResourceView* texture, const ImVec2& position, const ImVec2& size, std::function<void()> callback) {
	ImageButtonInfo imageButtonInfo;
	imageButtonInfo.label = "ImageButton";
	imageButtonInfo.texture = texture;
	imageButtonInfo.position = position;
	imageButtonInfo.size = size;
	imageButtonInfo.callback = callback;
	imageButtons.push_back(imageButtonInfo);
}

UIFrame& UIFrame::Position(ImVec2 newPosition)
{
	position = newPosition;
	return *this;
}

UIFrame& UIFrame::Size(ImVec2 newSize)
{
	size = newSize;
	return *this;
}

UIFrame& UIFrame::BackgroundColor(ImVec4 newColor)
{
	backgroundColor = newColor;
	return *this;
}

UIFrame& UIFrame::NoTitleBar(bool noTitle)
{
	noTitleBar = noTitle;
	return *this;
}

UIFrame& UIFrame::BorderLineTransparency(float transparency)
{
	borderLineTransparency = transparency;
	return *this;
}


void UIFrame::SetPosition(const ImVec2& newPosition)
{
	position = newPosition;
}

ImVec2 UIFrame::GetPosition() const
{
	return position;
}

void UIFrame::Render()
{
	ImGui::SetNextWindowPos(position, ImGuiCond_Always);
	ImGui::SetNextWindowSize(size, ImGuiCond_Once);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, backgroundColor);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, borderLineTransparency);

	auto defaultFlags = ImGuiWindowFlags_None |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;

	auto flags = noTitleBar ? defaultFlags | ImGuiWindowFlags_NoTitleBar : defaultFlags;

	ImGui::Begin(title.c_str(),	nullptr,flags);

	//ImGui::Begin(title.c_str());

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

	for (const auto& image : images) 
	{
		ImGui::SetCursorPos(image.position);
		ImGui::Image((ImTextureID)image.texture, image.size);
	}

	for (const auto& imageButton : imageButtons)
	{
		ImGui::SetCursorPos(imageButton.position);
		if (ImGui::ImageButton(imageButton.label.c_str(), (ImTextureID)(imageButton.texture), imageButton.size))
		{
			imageButton.callback();
		}
	}

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}
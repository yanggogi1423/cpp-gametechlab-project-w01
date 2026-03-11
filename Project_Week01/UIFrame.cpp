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

void UIFrame::AddText(const std::string& text, const ImVec2& position, ImFont* font, const ImVec4& color)
{
	TextInfo textInfo;
	textInfo.text = text;
	textInfo.position = position;
	textInfo.font = font;
	textInfo.color = color;

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

void UIFrame::AddImageButton(const std::string& text, ID3D11ShaderResourceView* texture, const ImVec2& position, const ImVec2& size, std::function<void()> callback) {
	ImageButtonInfo imageButtonInfo;
	imageButtonInfo.label = text;
	imageButtonInfo.texture = texture;
	imageButtonInfo.position = position;
	imageButtonInfo.size = size;
	imageButtonInfo.callback = callback;
	imageButtons.push_back(imageButtonInfo);
}


void UIFrame::AddSpriteButton(const std::string& text, ID3D11ShaderResourceView* texture, const ImVec2& position, const ImVec2& size, int index, std::function<void()> callback) {
	SpriteButtonInfo spriteInfo;
	spriteInfo.label = text;
	spriteInfo.texture = texture;
	spriteInfo.position = position;
	spriteInfo.size = size;
	spriteInfo.callback = callback;
	
	//80 * 128 이미지
	int columns = 80 / 16;
	int x = index % columns;
	int	y = index / columns;

	float u0 = (float)(x * 16) / 80;
	float v0 = (float)(y * 16) / 128;

	float u1 = (float)((x + 1) * 16) / 80;
	float v1 = (float)((y + 1) * 16) / 128;

	spriteInfo.uv0 = ImVec2(u0, v0);
	spriteInfo.uv1 = ImVec2(u1, v1);


	spriteButtons.push_back(spriteInfo);
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


	for (const auto& image : images) 
	{
		ImGui::SetCursorPos(image.position);
		ImGui::Image((ImTextureID)image.texture, image.size);
	}

	for (const auto& button : buttons)
	{
		ImGui::SetCursorPos(button.position);

		if (ImGui::Button(button.label.c_str(), button.size))
		{
			button.callback();
		}
	}

	for (const auto& imageButton : imageButtons)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 1, 1, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.2f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0.3f));

		ImGui::SetCursorPos(imageButton.position);
		if (ImGui::ImageButton(imageButton.label.c_str(), (ImTextureID)(imageButton.texture), imageButton.size))
		{
			imageButton.callback();
		}

		ImGui::PopStyleColor(3);
	}

	for (const auto& spriteButton : spriteButtons)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 1, 1, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.2f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0.3f));

		ImGui::SetCursorPos(spriteButton.position);
		if (ImGui::ImageButton(spriteButton.label.c_str(), (ImTextureID)(spriteButton.texture), spriteButton.size, spriteButton.uv0, spriteButton.uv1))
		{
			spriteButton.callback();
		}

		ImGui::PopStyleColor(3);
	}

	for (const auto& text : texts)
	{
		ImGui::PushFont(text.font);

		ImVec2 textSize = ImGui::CalcTextSize(text.text.c_str());

		ImVec2 pos = {
			text.position.x - textSize.x * 0.5f,
			text.position.y - textSize.y * 0.5f
		};

		ImGui::SetCursorPos(pos);

		ImGui::PushStyleColor(ImGuiCol_Text, text.color);
		ImGui::Text("%s", text.text.c_str());
		ImGui::PopStyleColor();

		ImGui::PopFont();
	}

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}
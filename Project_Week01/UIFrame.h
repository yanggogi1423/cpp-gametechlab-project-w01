#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include <vector>
#include <functional>
#include <string>
#include "datatype.h"
#include "UResourceManager.h"

struct ButtonInfo
{
	std::string label;
	ImVec2 position;
	ImVec2 size;
	std::function<void()> callback;
};

struct ImageButtonInfo
{
	std::string label;
	ID3D11ShaderResourceView* texture;
	ImVec2 position;
	ImVec2 size;
	std::function<void()> callback;
};

struct ImageInfo
{
	ID3D11ShaderResourceView* texture;
	ImVec2 position;
	ImVec2 size;
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
	std::vector<ImageInfo> images;
	std::vector<ImageButtonInfo> imageButtons;


public:
	UIFrame(const std::string& title);
	~UIFrame();

	void AddButton(const std::string& label, const ImVec2& position, const ImVec2& size, std::function<void()> callback);
	void AddText(const std::string& text, const ImVec2& position, ImFont* font);
	void AddImage(ID3D11ShaderResourceView* texture, const ImVec2& position, const ImVec2& size);
	void AddImageButton(ID3D11ShaderResourceView* texture, const ImVec2& position, const ImVec2& size, std::function<void()> callback);

	ImVec2 GetPosition() const;
	void SetPosition(const ImVec2& newPosition);
	void Render();

	UIFrame& Position(ImVec2 newPosition);
	UIFrame& Size(ImVec2 newSize);
	UIFrame& BackgroundColor(ImVec4 newColor);
	UIFrame& NoTitleBar(bool noTitle);
	UIFrame& BorderLineTransparency(float transparency);



};
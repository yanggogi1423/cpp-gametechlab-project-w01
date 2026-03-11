#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <random>

#include "WICTextureLoader/WICTextureLoader.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

class UResourceManager
{
private:
	ID3D11Device* Device;

	bool bIsInitialzed;

	//	Data 휘발 방지
	std::unordered_map<std::string, ID3D11ShaderResourceView*> TextureMap;

	//	Tip data
	std::vector<std::string> tipList;

public:
	//	Loading data
	std::vector<std::string> loadingList;

	//	How To Play
	std::vector<std::string> howToPlayList;

public:
	//	Image Resouces
	ID3D11ShaderResourceView* SRVBackground;
	ID3D11ShaderResourceView* SRVInGamePanel;
	ID3D11ShaderResourceView* SRVLeaderBoardPanel;
	ID3D11ShaderResourceView* SRVButtonSprite;

	//	Fonts Resources
	ImFont* FontLogo;
	ImFont* FontDefault;
	
public:
	UResourceManager() : SRVBackground(nullptr), SRVInGamePanel(nullptr), SRVLeaderBoardPanel(nullptr), SRVButtonSprite(nullptr),
		FontLogo(nullptr), FontDefault(nullptr),
		bIsInitialzed(false),
		Device(nullptr)
	{}

	void Initialize(ID3D11Device* device);
	void Release();
	bool LoadTexture(const std::string& key, const wchar_t* filePath);
	ID3D11ShaderResourceView* GetTexture(const std::string& key) const;
	bool IsInitialize() { return bIsInitialzed; }

	std::string GetRandomTips() const;
};


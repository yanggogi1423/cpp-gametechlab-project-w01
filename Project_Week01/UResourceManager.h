#pragma once

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
		bIsInitialzed(false)
	{}

	void Initialize(ID3D11Device* device);
	void Release();

	bool IsInitialize() { return bIsInitialzed; }
};


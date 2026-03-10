#include "UResourceManager.h"

//	Manager의 Boot에서 Call
void UResourceManager::Initialize(ID3D11Device* device)
{
	this->Device = device;

	//	시간 없어서 하드 코딩합니다.

	// 기본 텍스처 로드 (경로는 프로젝트에 맞게 수정)
	 LoadTexture("Background", L"res/image/Background.png");
	 LoadTexture("InGamePanel", L"res/image/InGamePanel.png");
	 LoadTexture("LeaderBoardPanel", L"res/image/LeaderBoardPanel.png");
	 LoadTexture("ButtonSprite", L"res/image/ButtonSprite.png");

	SRVBackground = GetTexture("Background");
	SRVInGamePanel = GetTexture("InGamePanel");
	SRVLeaderBoardPanel = GetTexture("LeaderBoardPanel");
	SRVButtonSprite = GetTexture("ButtonSprite");

	ImGuiIO& io = ImGui::GetIO();

	//	Logo Fonts
	FontLogo = io.Fonts->AddFontFromFileTTF
	(
		"res\\fonts\\ExtraBold.ttf",
		16.f,
		nullptr,
		io.Fonts->GetGlyphRangesDefault()
	);

	//	Default Fonts
	FontDefault = io.Fonts->AddFontFromFileTTF
	(
		"res\\fonts\\Medium.ttf",
		16.f,
		nullptr,
		io.Fonts->GetGlyphRangesDefault()
	);

	bIsInitialzed = true;
}

//	Manager의 Shutdown에서 Call
void UResourceManager::Release()
{
	if (!bIsInitialzed) return;

	SRVBackground->Release();
	SRVInGamePanel->Release();
	SRVLeaderBoardPanel->Release();
	SRVButtonSprite->Release();

	//	Font는 ImGui Release 시 자동으로 정리됨.
	//	따로 nullptr 초기화 안함.
}

bool UResourceManager::LoadTexture(const std::string& key, const wchar_t* filePath)
{
	if (!Device || !filePath)
		return false;

	auto found = TextureMap.find(key);
	if (found != TextureMap.end() && found->second)
		return true;

	ID3D11ShaderResourceView* srv = nullptr;
	HRESULT hr = DirectX::CreateWICTextureFromFile(Device, filePath, nullptr, &srv);
	if (FAILED(hr) || !srv)
		return false;

	TextureMap[key] = srv;
	return true;
}

ID3D11ShaderResourceView* UResourceManager::GetTexture(const std::string& key) const
{
	auto found = TextureMap.find(key);
	if (found == TextureMap.end())
		return nullptr;

	return found->second;
}
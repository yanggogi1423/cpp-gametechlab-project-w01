#include "UResourceManager.h"

//	Manager의 Boot에서 Call
void UResourceManager::Initialize(ID3D11Device* device)
{
	this->Device = device;

	//	시간 없어서 하드 코딩합니다.

	//	Background
	DirectX::CreateWICTextureFromFile(
		Device,
		L"res\\image\\Background.png",
		nullptr,
		&SRVBackground
	);

	//	InGame Panel
	DirectX::CreateWICTextureFromFile(
		Device,
		L"res\\image\\InGamePanel.png",
		nullptr,
		&SRVInGamePanel
	);

	//	LeaderBoard Panel
	DirectX::CreateWICTextureFromFile(
		Device,
		L"res\\image\\LeaderBoard.png",
		nullptr,
		&SRVLeaderBoardPanel
	);

	//	SRV Button Sprite
	DirectX::CreateWICTextureFromFile(
		Device,
		L"res\\image\\ButtonSprite.png",
		nullptr,
		&SRVButtonSprite
	);

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
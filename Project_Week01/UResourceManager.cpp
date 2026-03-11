#include "UResourceManager.h"

//	Manager의 Boot에서 Call
void UResourceManager::Initialize(ID3D11Device* device , ID3D11DeviceContext * deviceContext)
{
	this->Device = device;
	this->DeviceContext = deviceContext;

	/* Image Setting */

	// 기본 텍스처 로드 (경로는 프로젝트에 맞게 수정)
	 LoadTexture("Background", L"res/image/Background.png");
	 LoadTexture("InGamePanel", L"res/image/InGamePanel.png");
	 LoadTexture("LeaderBoardPanel", L"res/image/LeaderBoardPanel.png");
	 LoadTexture("ButtonSprite", L"res/image/ButtonSprite.png");



	 ImageLoadTexture(ImageName::ROCKET, L"res/image/rocket.png");
	 ImageLoadTexture(ImageName::SATURN, L"res/image/saturn.png");
	 ImageLoadTexture(ImageName::SATURN, L"res/image/blackplanet.png");
	 ImageLoadTexture(ImageName::SATURN, L"res/image/whiteplanet.png");
	 ImageLoadTexture(ImageName::SATURN, L"res/image/jupiter.png");
	 ImageLoadTexture(ImageName::SATURN, L"res/image/sun.png");
	 ImageLoadTexture(ImageName::GOAL, L"res/image/goal.png");

	SRVBackground = GetTexture("Background");
	SRVInGamePanel = GetTexture("InGamePanel");
	SRVLeaderBoardPanel = GetTexture("LeaderBoardPanel");
	SRVButtonSprite = GetTexture("ButtonSprite");

	/* Fonts Setting */
	ImGuiIO& io = ImGui::GetIO();

	//	Logo Fonts
	FontLogo = io.Fonts->AddFontFromFileTTF
	(
		"res\\fonts\\GravityBold8.ttf",
		108.f,
		nullptr,
		io.Fonts->GetGlyphRangesDefault()
	);

	//	Default Fonts
	FontDefault = io.Fonts->AddFontFromFileTTF
	(
		"res\\fonts\\GravityRegular5.ttf",
		32.f,
		nullptr,
		io.Fonts->GetGlyphRangesDefault()
	);

	FontInfoBold = io.Fonts->AddFontFromFileTTF
	(
		"res\\fonts\\NEXONLv1GothicBold.ttf",
		60.f,
		nullptr,
		io.Fonts->GetGlyphRangesKorean()
	);

	FontInfoRegular = io.Fonts->AddFontFromFileTTF
	(
		"res\\fonts\\NEXONLv1GothicRegular.ttf",
		36.f,
		nullptr,
		io.Fonts->GetGlyphRangesKorean()
	);

	FontInfoLight = io.Fonts->AddFontFromFileTTF
	(
		"res\\fonts\\NEXONLv1GothicLight.ttf",
		17.f,
		nullptr,
		io.Fonts->GetGlyphRangesKorean()
	);

	/* Tip List Setting */

	tipList.push_back("4일 만에 만들기에는 볼륨이 컸습니다.");
	tipList.push_back("랭킹을 올리려면 빠른 길을 선택해야 합니다.");
	tipList.push_back("잘 만들었죠?");
	tipList.push_back("정글은 밥이 맛있습니다.");
	tipList.push_back("식사 시간보다 25분 늦게 가면 안 기다려도 됩니다.");
	tipList.push_back("목요일 회식이 기대됩니다.");

	/* Loading List Setting */
	loadingList.push_back("Loading");
	loadingList.push_back("Loading.");
	loadingList.push_back("Loading..");
	loadingList.push_back("Loading...");

	/* How To Play List Setting */
	howToPlayList.push_back("[How To Play]");
	//	TODO : Make how to play

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

	for (int i = 0; i < ImageName::COUNT; i++)
	{
		if (TextureResources[i]) {
			TextureResources[i] = nullptr;
			TextureResources[i] -> Release();
		}
	}

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

std::string UResourceManager::GetRandomTips() const
{
	if (!bIsInitialzed) return "[UResourceManager] Please initialize first.";

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, tipList.size() - 1);

	return tipList[dist(mt)];
}

// texture 읽기
HRESULT UResourceManager::ImageLoadTexture(ImageName type, std::wstring filePath)
{
	ID3D11ShaderResourceView* srv = nullptr;
	HRESULT hr = DirectX::CreateWICTextureFromFile(Device, DeviceContext, filePath.c_str(), nullptr, &srv);

	if (SUCCEEDED(hr))
	{
		if (TextureResources[type]) TextureResources[type]->Release();
		TextureResources[type] = srv;
	}

	return hr;
}

ID3D11ShaderResourceView* UResourceManager::GetTexture(ImageName type)
{
	return TextureResources[type];
}

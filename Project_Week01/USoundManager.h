#pragma once

#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>

#include <string>
#include <unordered_map>
#include <vector>

#pragma comment(lib, "dsound")
#pragma comment(lib, "winmm")
#pragma comment(lib, "dxguid")

//	StageSelection Scene은 Main과 BGM 동일
enum class EBGM
{
	EBGM_Main,
	EBGM_InGameReady,
	EBGM_InGameRun,
	EBGM_Clear,
	EBGM_Defeat,
};

enum class ESFX
{
	ESFX_ButtonClick,
	ESFX_InGameStart
	
};

class USoundManager
{
private:
	//	Sound Struct (Inner)
	struct SoundClip
	{
		std::vector<IDirectSoundBuffer8*> Buffers;	//	동시에 여러 채널 재생 감안
		DWORD BufferSize = 0;
		float VolumeScale = 1.f;
		size_t NextIndex = 0;
	};

	/* Sound Data */
	std::unordered_map<EBGM, SoundClip> BgmClips;
	std::unordered_map<ESFX, SoundClip> SfxClips;

public:



private:
	bool CreatePrimaryBuffer();

	void CreateSoundClip(const std::string& filePath, int poolSize, SoundClip& clip);



public:
	USoundManager() = default;
	~USoundManager() = default;

	void Initialize(HWND hwnd);
	void Dispose();

	/* Features */
	void PlayBGM(EBGM bgm);
	void StopBGM();
	void StopBGM(float fadeOutTime);

	void PlaySFX(ESFX sfx);
	void StopSFX();
	void StopAllSFX();

	/* Setting */
	void SetBGMVolume();
	void SetSFXVolume();

	void GetBGMVolume();
	void GetSFXVolume();

};

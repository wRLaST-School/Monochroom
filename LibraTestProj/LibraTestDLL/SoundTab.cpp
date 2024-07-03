#include "SoundTab.h"
#include <Camera.h>
#include <SpDS.h>
#include <SpTextureManager.h>
#include <Util.h>
#include <SceneManager.h>
#include <Input.h>
#include <ConsoleWindow.h>
#include "GameManager.h"

void SoundTab::Init()
{
	mSoundOptionNum = 3;

	mSoundUIObj.resize(mSoundOptionNum);

	// 音量項目のオブジェクト設定
	mSoundUIObj[MASTER].planeObj = SceneManager::FindObject<Object3D>("MastarVolume");
	mSoundUIObj[BGM].planeObj = SceneManager::FindObject<Object3D>("BGMVolume");
	mSoundUIObj[SE].planeObj = SceneManager::FindObject<Object3D>("SEVolume");
}

void SoundTab::Update()
{
}

void SoundTab::Draw()
{
}

void SoundTab::OnUpdate()
{
	for (size_t i = 0; i < mSoundOptionNum; i++)
	{
		mSoundUIObj[i].planeObj->Activate();
	}

	mCurrentNum = 0;
}

void SoundTab::OffUpdate()
{
	for (size_t i = 0; i < mSoundOptionNum; i++)
	{
		mSoundUIObj[i].planeObj->Deactivate();
	}

	mCurrentNum = 0;
}

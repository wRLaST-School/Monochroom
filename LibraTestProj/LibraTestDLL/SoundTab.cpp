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

	mSoundSensivity.resize(mSoundOptionNum);

	mSoundNoneSelectScale = { 0.9f,0.6f,1 };
	mSoundSelectScale = { 1,0.8f,1 };

	mSoundDefuValue = 1;
	mSoundMaxValue = 10;

	mSoundUIObj.resize(mSoundOptionNum);

	mItemsParentObj = std::make_unique<Object3D>();
	mItemsParentObj.reset(SceneManager::FindObject<Object3D>("SoundTabItems"));

	// 音量項目のオブジェクト設定
	mSoundUIObj[MASTER].planeObj = SceneManager::FindObject<Object3D>("MastarVolume");
	mSoundUIObj[BGM].planeObj = SceneManager::FindObject<Object3D>("BGMVolume");
	mSoundUIObj[SE].planeObj = SceneManager::FindObject<Object3D>("SEVolume");
}

void SoundTab::Update()
{
	mSoundUIObj[mCurrentNum].state = SELECT;

	if (Input::Key::Triggered(DIK_W))
	{
		mCurrentNum--;
		if (mCurrentNum <= 0)
		{
			mCurrentNum = 0;
		}
		mSoundUIObj[mCurrentNum].state = SELECT;
	}
	if (Input::Key::Triggered(DIK_S))
	{
		mCurrentNum++;
		if (mCurrentNum >= mSoundOptionNum - 1)
		{
			mCurrentNum = mSoundOptionNum - 1;
		}
		mSoundUIObj[mCurrentNum].state = SELECT;
	}

	if (Input::Key::Triggered(DIK_A))
	{
		if (mSoundSensivity[mCurrentNum] > mSoundDefuValue)
		{
			mSoundSensivity[mCurrentNum]--;
		}
	}

	if (Input::Key::Triggered(DIK_D))
	{
		if (mSoundSensivity[mCurrentNum] < mSoundMaxValue)
		{
			mSoundSensivity[mCurrentNum]++;
		}
	}

	// それぞれの項目が選ばれているかどうか
	for (size_t i = 0; i < mSoundOptionNum; i++)
	{
		if (i != mCurrentNum)
		{
			mSoundUIObj[i].state = DISABLED;
		}

		switch (mSoundUIObj[i].state)
		{
		case DISABLED:
			mSoundUIObj[i].planeObj->scale = mSoundNoneSelectScale;
			break;
		case SELECT:
			mSoundUIObj[i].planeObj->scale = mSoundSelectScale;
			break;
		}

		mSoundUIObj[i].planeObj->Update();
	}

	ConsoleWindow::Log(std::format("サウンドアイテム値:{},{}\n", mCurrentNum, mSoundSensivity[mCurrentNum]));
}

void SoundTab::Draw()
{
}

void SoundTab::OnUpdate()
{
	mItemsParentObj->Activate();
	
	mCurrentNum = 0;
}

void SoundTab::OffUpdate()
{
	mItemsParentObj->Deactivate();

	mCurrentNum = 0;
}

#include "SoundTab.h"
#include <Camera.h>
#include <SpDS.h>
#include <SpTextureManager.h>
#include <Util.h>
#include <SceneManager.h>
#include <Input.h>
#include <ConsoleWindow.h>
#include "GameManager.h"
#include "AppOperationCommand.h"

void SoundTab::Init()
{
	mSoundOptionNum = 3;

	mSoundSensivity.resize(mSoundOptionNum);

	mSoundNoneSelectScale = { 0.98f,0.98f,1 };
	mSoundSelectScale = { 1,1,1 };

	mSoundDefuValue = 1;
	mSoundMaxValue = 5;

	mSoundUIObj.resize(mSoundOptionNum);

	mItemsParentObj = SceneManager::FindObject<Object3D>("SoundTabItems");

	// 音量項目のオブジェクト設定
	mSoundUIObj[MASTER].planeObj = SceneManager::FindObject<Object3D>("MasterParent");
	mSoundUIObj[BGM].planeObj = SceneManager::FindObject<Object3D>("BGMParent");
	mSoundUIObj[SE].planeObj = SceneManager::FindObject<Object3D>("SEParent");

	mNumberTex.resize(10);
	mNumberTex = {
		"zero",
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine"
	};

	for (size_t i = 0; i < mSoundOptionNum; i++)
	{
		mSoundSensivity[i] = 1;
		mSoundUIObj[i].numObj = SceneManager::FindChildObject<Object3D>("Num", mSoundUIObj[i].planeObj);
	}

	mSoundCurrentNum = 0;
}

void SoundTab::Update()
{

}

void SoundTab::Draw()
{
}

void SoundTab::CopyComponent(IComponent* src)
{
}

void SoundTab::MenuUpdate()
{
	mSoundUIObj[mSoundCurrentNum].state = SELECT;

	if (AppOperationCommand::GetInstance()->UISelectUpCommand())
	{
		mSoundCurrentNum--;
		if (mSoundCurrentNum <= 0)
		{
			mSoundCurrentNum = 0;
		}
		mSoundUIObj[mSoundCurrentNum].state = SELECT;
	}
	if (AppOperationCommand::GetInstance()->UISelectDownCommand())
	{
		mSoundCurrentNum++;
		if (mSoundCurrentNum >= mSoundOptionNum - 1)
		{
			mSoundCurrentNum = mSoundOptionNum - 1;
		}
		mSoundUIObj[mSoundCurrentNum].state = SELECT;
	}

	if (AppOperationCommand::GetInstance()->UISelectLeftCommand())
	{
		if (mSoundSensivity[mSoundCurrentNum] > mSoundDefuValue)
		{
			mSoundSensivity[mSoundCurrentNum]--;
		}
	}

	if (AppOperationCommand::GetInstance()->UISelectRightCommand())
	{
		if (mSoundSensivity[mSoundCurrentNum] < mSoundMaxValue)
		{
			mSoundSensivity[mSoundCurrentNum]++;
		}
	}

	// それぞれの項目が選ばれているかどうか
	for (size_t i = 0; i < mSoundOptionNum; i++)
	{
		if (i != mSoundCurrentNum)
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

		mSoundUIObj[i].numObj->texture = mNumberTex[(int)mSoundSensivity[i]];

		mSoundUIObj[i].numObj->Update();

		mSoundUIObj[i].planeObj->Update();
	}

	ConsoleWindow::Log(std::format("サウンドアイテム値:{},{}\n", mSoundCurrentNum, mSoundSensivity[mSoundCurrentNum]));
}

void SoundTab::OnUpdate()
{
	mItemsParentObj->Activate();
	mSoundCurrentNum = 0;
	MenuUpdate();
}

void SoundTab::OffUpdate()
{
	mItemsParentObj->Deactivate();

}
RegisterScriptBody(SoundTab);
#include "CameraTab.h"
#include <Camera.h>
#include <SpDS.h>
#include <SpTextureManager.h>
#include <Util.h>
#include <SceneManager.h>
#include <Input.h>
#include <ConsoleWindow.h>
#include "GameManager.h"

void CameraTab::Init()
{
	mCameraOptionNum = 4;

	mCameraSensivity.resize(mCameraOptionNum);

	mCameraNoneSelectScale = { 0.9f,0.6f,1 };
	mCameraSelectScale = { 1,0.8f,1 };

	mCameraDefuValue = 1;
	mCameraMaxValue = 5;

	mCameraUIObj.resize(mCameraOptionNum);

	mItemsParentObj = std::make_unique<Object3D>();
	mItemsParentObj.reset(SceneManager::FindObject<Object3D>("CameraTabItems"));

	// カメラ項目のオブジェクト設定
	mCameraUIObj[MOUSESENSITIVITY_X].planeObj = SceneManager::FindObject<Object3D>("MouseSensitivityX");
	mCameraUIObj[MOUSESENSITIVITY_Y].planeObj = SceneManager::FindObject<Object3D>("MouseSensitivityY");
	mCameraUIObj[PADSENSITIVITY_X].planeObj = SceneManager::FindObject<Object3D>("PadSensitivityX");
	mCameraUIObj[PADSENSITIVITY_Y].planeObj = SceneManager::FindObject<Object3D>("PadSensitivityY");
}

void CameraTab::Update()
{
	mCameraUIObj[mCurrentNum].state = SELECT;

	if (Input::Key::Triggered(DIK_W))
	{
		mCurrentNum--;
		if (mCurrentNum <= 0)
		{
			mCurrentNum = 0;
		}
		mCameraUIObj[mCurrentNum].state = SELECT;
	}
	if (Input::Key::Triggered(DIK_S))
	{
		mCurrentNum++;
		if (mCurrentNum >= mCameraOptionNum - 1)
		{
			mCurrentNum = mCameraOptionNum - 1;
		}
		mCameraUIObj[mCurrentNum].state = SELECT;
	}

	if (Input::Key::Triggered(DIK_A))
	{
		if (mCameraSensivity[mCurrentNum] > mCameraDefuValue)
		{
			mCameraSensivity[mCurrentNum]--;
		}
	}

	if (Input::Key::Triggered(DIK_D))
	{
		if (mCameraSensivity[mCurrentNum] < mCameraMaxValue)
		{
			mCameraSensivity[mCurrentNum]++;
		}
	}

	// それぞれの項目が選ばれているかどうか
	for (size_t i = 0; i < mCameraOptionNum; i++)
	{
		if (i != mCurrentNum) 
		{
			mCameraUIObj[i].state = DISABLED;
		}

		switch (mCameraUIObj[i].state)
		{
		case DISABLED:
			mCameraUIObj[i].planeObj->scale = mCameraNoneSelectScale;
			break;
		case SELECT:
			mCameraUIObj[i].planeObj->scale = mCameraSelectScale;
			break;
		}

		mCameraUIObj[i].planeObj->Update();
	}

	ConsoleWindow::Log(std::format("カメラアイテム値:{},{}\n", mCurrentNum, mCameraSensivity[mCurrentNum]));
}

void CameraTab::Draw()
{
}

void CameraTab::OnUpdate()
{
	mItemsParentObj->Activate();
	
	mCurrentNum = 0;
}

void CameraTab::OffUpdate()
{
	mItemsParentObj->Deactivate();

	mCurrentNum = 0;
}

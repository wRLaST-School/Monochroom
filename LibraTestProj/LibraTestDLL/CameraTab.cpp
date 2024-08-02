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
	mCameraNoneSelectScale = { 0.98f,0.98f,1 };
	mCameraSelectScale = { 1,1,1 };

	mCameraDefuValue = 1;
	mCameraMaxValue = 5;

	mCameraUIObj.resize(mCameraOptionNum);

	mItemsParentObj = SceneManager::FindObject<Object3D>("CameraTabItems");

	// カメラ項目のオブジェクト設定
	mCameraUIObj[MOUSESENSITIVITY_X].planeObj = SceneManager::FindObject<Object3D>("MouseXParent");
	mCameraUIObj[MOUSESENSITIVITY_Y].planeObj = SceneManager::FindObject<Object3D>("MouseYParent");
	mCameraUIObj[PADSENSITIVITY_X].planeObj = SceneManager::FindObject<Object3D>("PadXParent");
	mCameraUIObj[PADSENSITIVITY_Y].planeObj = SceneManager::FindObject<Object3D>("PadYParent");

	SpTextureManager::LoadDiv("Assets/Images/numbers1.png", (int)320, (int)320, 10, 1, mNumberTex);

	for (size_t i = 0; i < mCameraOptionNum; i++)
	{
		mCameraSensivity[i] = 1;
		mCameraUIObj[i].numObj = SceneManager::FindChildObject<Object3D>("Num", mCameraUIObj[i].planeObj);
	}

	mCameraCurrentNum = 0;
}

void CameraTab::Update()
{

}

void CameraTab::Draw()
{
}

void CameraTab::CopyComponent(IComponent* src)
{
}

void CameraTab::MenuUpdate()
{
	mCameraUIObj[mCameraCurrentNum].state = SELECT;

	if (Input::Key::Triggered(DIK_W))
	{
		mCameraCurrentNum--;
		if (mCameraCurrentNum <= 0)
		{
			mCameraCurrentNum = 0;
		}
		mCameraUIObj[mCameraCurrentNum].state = SELECT;
	}
	if (Input::Key::Triggered(DIK_S))
	{
		mCameraCurrentNum++;
		if (mCameraCurrentNum >= mCameraOptionNum - 1)
		{
			mCameraCurrentNum = mCameraOptionNum - 1;
		}
		mCameraUIObj[mCameraCurrentNum].state = SELECT;
	}

	if (Input::Key::Triggered(DIK_A))
	{
		if (mCameraSensivity[mCameraCurrentNum] > mCameraDefuValue)
		{
			mCameraSensivity[mCameraCurrentNum]--;
		}
	}

	if (Input::Key::Triggered(DIK_D))
	{
		if (mCameraSensivity[mCameraCurrentNum] < mCameraMaxValue)
		{
			mCameraSensivity[mCameraCurrentNum]++;
		}
	}

	// それぞれの項目が選ばれているかどうか
	for (size_t i = 0; i < mCameraOptionNum; i++)
	{
		if (i != mCameraCurrentNum)
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

		mCameraUIObj[i].numObj->texture = mNumberTex[(int)mCameraSensivity[i]];

		mCameraUIObj[i].numObj->Update();
		mCameraUIObj[i].planeObj->Update();
	}

	mMouseSensivity = { mCameraSensivity[MOUSESENSITIVITY_X],mCameraSensivity[MOUSESENSITIVITY_Y] };
	mPadSensivity = { mCameraSensivity[PADSENSITIVITY_X],mCameraSensivity[PADSENSITIVITY_Y] };

	ConsoleWindow::Log(std::format("カメラアイテム値: {},{}\n", mCameraCurrentNum, mCameraSensivity[mCameraCurrentNum]));
}

void CameraTab::OnUpdate()
{
	mItemsParentObj->Activate();
	mCameraCurrentNum = 0;
	MenuUpdate();
}

void CameraTab::OffUpdate()
{
	mItemsParentObj->Deactivate();
}

Vec2 CameraTab::GetMouseSensivity()
{
	Vec2 result = mMouseSensivity;

	return result;
}

Vec2 CameraTab::GetPadSensivity()
{
	Vec2 result = mPadSensivity;

	return result;
}

RegisterScriptBody(CameraTab);
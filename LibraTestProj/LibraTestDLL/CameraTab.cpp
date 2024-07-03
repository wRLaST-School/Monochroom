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

	mCameraUIObj.resize(mCameraOptionNum);

	// カメラ項目のオブジェクト設定
	mCameraUIObj[MOUSESENSITIVITY_X].planeObj = SceneManager::FindObject<Object3D>("MouseSensitivityX");
	mCameraUIObj[MOUSESENSITIVITY_Y].planeObj = SceneManager::FindObject<Object3D>("MouseSensitivityY");
}

void CameraTab::Update()
{
}

void CameraTab::Draw()
{
}

void CameraTab::OnUpdate()
{
	for (size_t i = 0; i < mCameraOptionNum; i++)
	{
		mCameraUIObj[i].planeObj->Activate();
	}

	mCurrentNum = 0;
}

void CameraTab::OffUpdate()
{
	for (size_t i = 0; i < mCameraOptionNum; i++)
	{
		mCameraUIObj[i].planeObj->Deactivate();
	}

	mCurrentNum = 0;
}

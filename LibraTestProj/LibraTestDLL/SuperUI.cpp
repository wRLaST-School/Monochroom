#include "SuperUI.h"
#include <Camera.h>
#include <SpDS.h>
#include <SpTextureManager.h>
#include <Util.h>
#include <SceneManager.h>
#include <Input.h>
#include <ConsoleWindow.h>

void SuperUI::Init()
{

	//testObj = SceneManager::FindObject<Object3D>("Object3D");

	// フラグ関連の初期化
	mIsOpenUIMenu = false;

	// テクスチャのロード
	LoadTexInit();

	// 初期のベース座標を設定
	mUiOptionBasePos = { 200,100 };
	mUiOptionItemBasePos = { Util::GetWinHalfSize().x,300 };

	// UIのオプションの項目数
	mNumOfUIOption = 5;

	// UIのスケールの変化量の初期設定
	mUIStatus.resize(mNumOfUIOption);
	mEaseUIMenu.resize(mNumOfUIOption);

	// UIオプションの座標を設定
	for (size_t i = 0; i < mNumOfUIOption; i++)
	{
		mUIStatus[i].basePos.x = 100 * i;
		mUIStatus[i].basePos.y = 150;
	}

	for (size_t i = 0; i < mEaseUIMenu.size(); i++)
	{
		mEaseUIMenu[i].SetEaseTimer(90);
	}
}

void SuperUI::Update()
{
	// もしTABキーを押したらメニューを開け閉めする
	if (Input::Key::Triggered(DIK_TAB)) {
		ConsoleWindow::Log("TAB押された。");
		if (mIsOpenUIMenu) {
			mIsOpenUIMenu = false;
			mIsMomentOpenMenu = false;
		}
		else {
			mIsMomentOpenMenu = true;
		}
	}

	// メニューが開いた瞬間 
	if (mIsMomentOpenMenu) {
		MomentMenuReset();
	}

	// メニューがオンになったら
	if (mIsOpenUIMenu) {
		
	}

}

void SuperUI::Draw()
{
	SpDS::DrawRotaGraph(100, 100, 1, 1, 0, "testTex");

	for (size_t i = 0; i < mNumOfUIOption; i++)
	{
		SpDS::DrawRotaGraph(mUIStatus[i].basePos.x, mUIStatus[i].basePos.y, 
							mUIStatus[i].scaleChangeValue.x, mUIStatus[i].scaleChangeValue.y, 0, "testTex");
	}
	
}

void SuperUI::CopyComponent(IComponent* src)
{

}

void SuperUI::LoadTexInit()
{
	SpTextureManager::LoadTexture("Assets/Images/DissolveMap.png", "testTex");
}

void SuperUI::MomentMenuReset()
{
	for (size_t i = 0; i < mNumOfUIOption; i++)
	{
		if (mUiWaitTimer > mUiWaitTimeInterval * i) {
			mUIStatus[i].IsTimingOff = true;
		}

		// タイミングをずらして、UIの項目を出現させる
		if (mUIStatus[i].IsTimingOff == true) {
			mEaseUIMenu[i].Update();

			mUIStatus[i].scaleChangeValue = mEaseUIMenu[i].In(1, 2);

			if (mEaseUIMenu[i].GetisEnd()) {
				mIsMomentOpenMenu = false;
				mIsOpenUIMenu = true;
				mEaseUIMenu[i].Reset();
			}
		}
	}
}

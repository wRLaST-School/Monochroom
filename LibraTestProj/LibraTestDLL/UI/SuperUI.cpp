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
	mNumOfUIOption = 3;

	// UIのスケールの変化量の初期設定
	mUIStatus.resize(mNumOfUIOption);
	mEaseUIMenu.resize(mNumOfUIOption);

	mUICommonBasePos = 420;
	mUICommonSpacePos = 120;

	mUICurrentNum = 0;

	mUIDesabledScale = { 1 ,0.3f};
	mUISelectScale = { 1.1f ,0.4f };

	mDesabledColor = { 150,150,150 };
	mSelectColor = { 255,255,255 }; 
	mPressedColor = { 100,255,100 };

	// UIオプションの座標を設定
	for (size_t i = 0; i < mNumOfUIOption; i++)
	{
		mUIStatus[i].afterBasePos.x = 200;
		mUIStatus[i].afterBasePos.y = mUICommonBasePos + (mUICommonSpacePos * i);
		mUIStatus[i].beforePos.x = -300;
		mUIStatus[i].beforePos.y = mUICommonBasePos + (mUICommonSpacePos * i);
		mUIStatus[i].scaleChangeValue = mUIDesabledScale;
	}

	for (size_t i = 0; i < mNumOfUIOption; i++)
	{
		mEaseUIMenu[i].SetEaseTimer(30);
		mEaseUIMenu[i].Reset();
	}
}

void SuperUI::Update()
{
	
	// もしTABキーを押したらメニューを開け閉めする
	if (Input::Key::Triggered(DIK_TAB)) {
		ConsoleWindow::Log("TAB押された。");
		if (mIsDisplayUI) {
			mIsOpenUIMenu = false;
			mIsMomentOpenMenu = false;
			mIsDisplayUI = false;
			ResetOption();
			ConsoleWindow::Log("メニューを閉じた");
		}
		else {
			mIsMomentOpenMenu = true;
			mIsDisplayUI = true;
			ConsoleWindow::Log("メニューを開いた");
		}
	}

	// メニューが開いた瞬間 
	if (mIsMomentOpenMenu) {
		MomentMenuReset();
	}

	// メニューがオンになったら
	if (mIsOpenUIMenu) {
		UIOptionsUpdate();
	}

}

void SuperUI::Draw()
{
	//SpDS::DrawRotaGraph(100, 100, 1, 1, 0, "testTex");

	if (mIsDisplayUI) {
		for (size_t i = 0; i < mNumOfUIOption; i++)
		{
			SpDS::DrawRotaGraph(mUIStatus[i].posChangeValue.x, mUIStatus[i].posChangeValue.y,
								mUIStatus[i].scaleChangeValue.x, mUIStatus[i].scaleChangeValue.y, 0, "testTex",
								Anchor::Center, mUIStatus[i].correntColor);
		}
	}
}

void SuperUI::CopyComponent(IComponent* src)
{

}

void SuperUI::LoadTexInit()
{
	SpTextureManager::LoadTexture("Assets/Images/circleParticle.png", "testTex");
}

void SuperUI::MomentMenuReset()
{
	ConsoleWindow::Log("メニューが開いた瞬間。");

	mUiWaitTimer++;

	for (size_t i = 0; i < mNumOfUIOption; i++)
	{
		if (mUiWaitTimer > mUiWaitTimeInterval * i) {
			mUIStatus[i].IsTimingOff = true;
		}

		// タイミングをずらして、UIの項目を出現させる
		if (mUIStatus[i].IsTimingOff == true) {
			mEaseUIMenu[i].Update();

			mUIStatus[i].posChangeValue = mEaseUIMenu[i].In(mUIStatus[i].beforePos, mUIStatus[i].afterBasePos);

			if (mEaseUIMenu[mNumOfUIOption-1].GetisEnd()) {
				mIsMomentOpenMenu = false;
				mIsOpenUIMenu = true;
				mUiWaitTimer = 0;
				mEaseUIMenu[i].Reset();
			}
		}
	}
}

void SuperUI::ResetOption()
{
	mUiWaitTimer = 0;
	mUICurrentNum = 0;
	for (size_t i = 0; i < mNumOfUIOption; i++)
	{
		mEaseUIMenu[i].Reset();
		mUIStatus[i].posChangeValue = mUIStatus[i].beforePos;
		mUIStatus[i].scaleChangeValue = mUIDesabledScale;
		mUIStatus[i].IsTimingOff = false;
		mUIStatus[i].IsActiveMenu = false;
	}
}

void SuperUI::UIOptionsUpdate()
{
	for (size_t i = 0; i < mNumOfUIOption; i++)
	{
		// 選択されたときとそうではないとき
		if (mUIStatus[mUICurrentNum].state != PRESSED)
		{
			mUIStatus[mUICurrentNum].state = SELECT;
			ConsoleWindow::Log(std::format("今のメニュー番号:{}\n", mUICurrentNum));

			if (Input::Key::Triggered(DIK_SPACE))
			{
				mUIStatus[mUICurrentNum].state = PRESSED;
			}

			// 項目のナンバーを変更
			if (Input::Key::Triggered(DIK_W)) {
				mUICurrentNum--;
				if (mUICurrentNum <= 0) {
					mUICurrentNum = 0;
				}
				mUIStatus[mUICurrentNum].state = SELECT;

				break;
			}
			if (Input::Key::Triggered(DIK_S)) {
				mUICurrentNum++;
				if (mUICurrentNum >= mNumOfUIOption - 1) {
					mUICurrentNum = mNumOfUIOption - 1;
				}
				mUIStatus[mUICurrentNum].state = SELECT;
				break;
			}
		}
		else {
			if (Input::Key::Triggered(DIK_X)) {
				mUIStatus[mUICurrentNum].state = SELECT;
			}
		}
	}

	for (size_t i = 0; i < mNumOfUIOption; i++)
	{
		// 選択されたときとそうではないとき
		if (mUIStatus[i].state != PRESSED)
		{
			if (i != mUICurrentNum) {
				mUIStatus[i].state = DISABLED;
			}
		}

		// それぞれの項目の状態処理
		switch (mUIStatus[i].state)
		{
		case DISABLED:
		{
			mUIStatus[i].scaleChangeValue = mUIDesabledScale;
			mUIStatus[i].correntColor = mDesabledColor;
		}
			break;
		case SELECT:
		{
			mUIStatus[i].scaleChangeValue = mUISelectScale;
			mUIStatus[i].correntColor = mSelectColor;
		}
			break;
		case PRESSED:
		{
			mUIStatus[i].correntColor = mPressedColor;
		}
			break;
		default:
			break;
		}
	}

}

RegisterScriptBody(SuperUI);
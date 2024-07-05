#include "SuperUI.h"
#include <Camera.h>
#include <SpDS.h>
#include <SpTextureManager.h>
#include <Util.h>
#include <SceneManager.h>
#include <Input.h>
#include <ConsoleWindow.h>
#include "GameManager.h"

float DegreeToRadian(float angle)
{
	float radian = angle * (float)(PI / 180);

	return radian;
}


void SuperUI::Init()
{

	//testObj = SceneManager::FindObject<Object3D>("Object3D");

	// フラグ関連の初期化
	mIsOpenUIMenu = false;

	// テクスチャのロード
	//LoadTexInit();

	mUICurrentNum = 0;

	mUIDesabledScale = { 0.6f ,0.1f };
	mUISelectScale = { 0.7f ,0.12f };

	mDesabledColor = { 200,0,0 };
	mSelectColor = { 255,255,255 };
	mPressedColor = { 150,225,255 };

	mDesabledTextColor = { 215,215,215 };
	mSelectTextColor = { 255,255,255 };
	mPressedTextColor = { 255,255,255 };

	mUITextBeforeColor = { 215,215,215,0 };
	mUITextAfterColor = { 245,245,245,255 };

	mUIMenuTextPos = { 160,360 };
	mUIMenuTexScale = { 0.5f,0.4f };

	mUIBoardPos = { 940,400 };
	mUIBoardScale = { 2.4f,4.4f };
	mUIBoardCurrentColor = mDesabledColor;



	UIObj3DInit();
	//Input::Mouse::HideCursor();
}

void SuperUI::Update()
{
	//mMenuParentObj->position.x += 0.05f;
	//mMenuParentObj->Update();
	// もしTABキーを押したらメニューを開け閉めする
	if (Input::Key::Triggered(DIK_ESCAPE))
	{
		ConsoleWindow::Log("TAB押された。");
		if (mIsDisplayUI)
		{
			SceneManager::FindObject<GameManager>("GameManager")->SetIsStop(false);
			mIsOpenUIMenu = false;
			mIsMomentOpenMenu = false;
			mIsDisplayUI = false;
			ResetOption();
			UIMainMenuOffReset();
			ConsoleWindow::Log("メニューを閉じた");
		}
		else
		{
			SceneManager::FindObject<GameManager>("GameManager")->SetIsStop(true);
			mIsMomentOpenMenu = true;
			mIsDisplayUI = true;
			UIMainMenuOnReset();

			ConsoleWindow::Log("メニューを開いた");
		}
	}

	// メニューが開いた瞬間 
	if (mIsMomentOpenMenu)
	{
		MomentMenuReset();
	}

	if (mIsDisplayUI)
	{
		UIObj3DUpdate();
		ConsoleWindow::Log("メニューを開いた");
	}

	// メニューがオンになったら
	if (mIsOpenUIMenu)
	{




		UIOptionsUpdate();
	}

}

void SuperUI::Draw()
{

}

void SuperUI::CopyComponent(IComponent* src)
{

}

void SuperUI::UIObj3DInit()
{

	// 項目数の初期化
	mNumMenu = 4;

	mNumOption = 3;

	mTabRotaAfter = -10;

	mTabRotaBefore = 0;

	mTabBoardRotaAfter = -15;

	mTabBoardRotaBefore = 0;

	mTabEaseTimeLimit = 30;

	IsUITabOn = false;

	mCurrentTabNum = 0;

	mUITabEase.SetEaseTimer((int32_t)mTabEaseTimeLimit);

	mUITabBoardEase.SetEaseTimer((int32_t)(mTabEaseTimeLimit * 1.5f));

	// メニューのリストをリサイズ
	mMenuUIObj.resize(mNumMenu);
	mMenuTabUIObj.resize(mNumOption);
	mTabItems.resize(mNumOption);
	

	// シーンに配置されているオブジェクトを走査して、代入

	mMenuParentObj = std::make_unique<Object3D>();
	mTabsParentObj = std::make_unique<Object3D>();
	mTabBoardObj = std::make_unique<Object3D>();
	mMenuPlaneObj = std::make_unique<Object3D>();
	mPlanesParentObj = std::make_unique<Object3D>();

	mCameraItem = std::make_unique<CameraTab>();
	mGpraphicsItem = std::make_unique<GraphicsTab>();
	mSoundItem = std::make_unique<SoundTab>();

	mTabItems.push_back(*mCameraItem.get());
	mTabItems.push_back(*mGpraphicsItem.get());
	mTabItems.push_back(*mSoundItem.get());

	for (size_t i = 0; i < mNumOption; i++)
	{
		mTabItems[i].Init();
	}

	mMenuParentObj.reset(SceneManager::FindObject<Object3D>("UIParentObj"));
	mTabsParentObj.reset(SceneManager::FindObject<Object3D>("Tabs"));
	mTabBoardObj.reset(SceneManager::FindObject<Object3D>("Board"));
	mMenuPlaneObj.reset(SceneManager::FindObject<Object3D>("MenuPlane"));
	mPlanesParentObj.reset(SceneManager::FindObject<Object3D>("Planes"));

	// メインメニューの項目のオブジェクト設定
	mMenuUIObj[GUID].planeObj = SceneManager::FindObject<Object3D>("GuidPlane");
	mMenuUIObj[OPTIONS].planeObj = SceneManager::FindObject<Object3D>("OptionPlane");
	mMenuUIObj[QUIT_SELECT].planeObj = SceneManager::FindObject<Object3D>("SelectPlane");
	mMenuUIObj[QUIT_TITLE].planeObj = SceneManager::FindObject<Object3D>("QuitTitlePlane");

	// オプションメニューの項目のオブジェクト設定
	mMenuTabUIObj[CAMERA].planeObj = SceneManager::FindObject<Object3D>("CameraTab");
	mMenuTabUIObj[GRAPHICS].planeObj = SceneManager::FindObject<Object3D>("GraohicsTab");
	mMenuTabUIObj[SOUND].planeObj = SceneManager::FindObject<Object3D>("SoundTab");
}

void SuperUI::UIObj3DUpdate()
{

	UIMainMenuUpdate();

	UITabMenuUpdate();
}

void SuperUI::LoadTexInit()
{
	SpTextureManager::LoadTexture("Assets/Images/circleParticle.png", "testTex");
	SpTextureManager::LoadTexture("Assets/Images/MenuImages/ButtonBack.png", "buttonBackTex");
	SpTextureManager::LoadTexture("Assets/Images/MenuImages/CameraText.png", "cameraTextTex");
	SpTextureManager::LoadTexture("Assets/Images/MenuImages/GuidText.png", "guidTextTex");
	SpTextureManager::LoadTexture("Assets/Images/MenuImages/QuitText.png", "quitTextTex");
	SpTextureManager::LoadTexture("Assets/Images/MenuImages/MenuText.png", "menuTextTex");
}

void SuperUI::MomentMenuReset()
{
	//ConsoleWindow::Log("メニューが開いた瞬間。");

	//mUiWaitTimer++;

	//for (size_t i = 0; i < mNumOfUIOption; i++)
	//{
	//	if (mUiWaitTimer > mUiWaitTimeInterval * i) 
	//	{
	//		mUIStatus[i].IsTimingOff = true;
	//	}

	//	// タイミングをずらして、UIの項目を出現させる
	//	if (mUIStatus[i].IsTimingOff == true)
	//	{
	//		mEaseUIMenu[i].Update();

	//		mUIStatus[i].posChangeValue = mEaseUIMenu[i].In(mUIStatus[i].beforePos, mUIStatus[i].afterBasePos);

	//		if (mEaseUIMenu[mNumOfUIOption-1].GetisEnd()) 
	//		{
	//			mIsMomentOpenMenu = false;
	//			mIsOpenUIMenu = true;
	//			mUiWaitTimer = 0;
	//			mEaseUIMenu[i].Reset();
	//		}
	//	}
	//}
}

void SuperUI::ResetOption()
{
	//mUiWaitTimer = 0;
	//mUICurrentNum = 0;
	//for (size_t i = 0; i < mNumOfUIOption; i++)
	//{
	//	mEaseUIMenu[i].Reset();
	//	mUIStatus[i].posChangeValue = mUIStatus[i].beforePos;
	//	mUIStatus[i].scaleChangeValue = mUIDesabledScale;
	//	mUIStatus[i].IsTimingOff = false;
	//	mUIStatus[i].IsActiveMenu = false;
	//	mUIStatus[i].IsPressedAlpha = false;
	//}
}

void SuperUI::UIOptionsUpdate()
{
	//for (size_t i = 0; i < mNumOfUIOption; i++)
	//{
	//	 選択されたときとそうではないとき
	//	if (mUIStatus[mUICurrentNum].state != PRESSED)
	//	{
	//		mUIStatus[mUICurrentNum].state = SELECT;
	//		ConsoleWindow::Log(std::format("今のメニュー番号:{}\n", mUICurrentNum));

	//		if (Input::Key::Triggered(DIK_Z))
	//		{
	//			mUIStatus[mUICurrentNum].state = PRESSED;
	//		}

	//		 項目のナンバーを変更
	//		if (Input::Key::Triggered(DIK_W)) 
	//		{
	//			mUICurrentNum--;
	//			if (mUICurrentNum <= 0) 
	//			{
	//				mUICurrentNum = 0;
	//			}
	//			mUIStatus[mUICurrentNum].state = SELECT;

	//			break;
	//		}
	//		if (Input::Key::Triggered(DIK_S)) 
	//		{
	//			mUICurrentNum++;
	//			if (mUICurrentNum >= mNumOfUIOption - 1) 
	//			{
	//				mUICurrentNum = mNumOfUIOption - 1;
	//			}
	//			mUIStatus[mUICurrentNum].state = SELECT;
	//			break;
	//		}
	//	}
	//	else 
	//	{
	//		if (Input::Key::Triggered(DIK_X) && mUIStatus[mUICurrentNum].IsPressedAlpha) 
	//		{
	//			mUIBoardCurrentColor = mDesabledColor;
	//			mUIStatus[mUICurrentNum].state = SELECT;
	//			mUIStatus[mUICurrentNum].IsPressedAlpha = false;
	//			mUIStatus[mUICurrentNum].textCurrentColor = mUITextBeforeColor;
	//		}
	//	}
	//}

	//for (size_t i = 0; i < mNumOfUIOption; i++)
	//{
	//	 選択されたときとそうではないとき
	//	if (mUIStatus[i].state != PRESSED)
	//	{
	//		if (i != mUICurrentNum) 
	//		{
	//			mUIStatus[i].state = DISABLED;
	//		}
	//	}

	//	 それぞれの項目の状態処理
	//	switch (mUIStatus[i].state)
	//	{
	//	case DISABLED:
	//	{
	//		mUIStatus[i].scaleChangeValue = mUIDesabledScale;
	//		mUIStatus[i].correntColor = mDesabledTextColor;
	//		mUIStatus[i].buttonColor = mDesabledColor;
	//	}
	//		break;
	//	case SELECT:
	//	{
	//		mUIStatus[i].scaleChangeValue = mUISelectScale;
	//		mUIStatus[i].correntColor = mSelectTextColor;
	//		mUIStatus[i].buttonColor = mSelectColor;
	//	}
	//		break;
	//	case PRESSED:
	//	{
	//		mUIStatus[i].correntColor = mPressedTextColor;
	//		mUIStatus[i].buttonColor = mPressedColor;

	//		if (mUIStatus[i].IsPressedAlpha == false) 
	//		{
	//			mTextAlphaEase.Update();

	//			mUIBoardCurrentColor.f4.x = mTextAlphaEase.In(mDesabledColor.f4.x, mSelectColor.f4.x);
	//			mUIBoardCurrentColor.f4.y = mTextAlphaEase.In(mDesabledColor.f4.y, mSelectColor.f4.y);
	//			mUIBoardCurrentColor.f4.z = mTextAlphaEase.In(mDesabledColor.f4.z, mSelectColor.f4.z);

	//			mUIStatus[i].textCurrentColor.f4.w = mTextAlphaEase.In(mUITextBeforeColor.f4.w, mUITextAfterColor.f4.w);

	//			if (mTextAlphaEase.GetisEnd()) 
	//			{
	//				mTextAlphaEase.Reset();
	//				mUIStatus[i].IsPressedAlpha = true;
	//			}
	//		}
	//		
	//	}
	//		break;
	//	default:
	//		break;
	//	}
	//}

}

void SuperUI::UIMainMenuUpdate()
{
	for (size_t i = 0; i < mNumMenu; i++)
	{		// 選択されたときとそうではないとき
		if (mMenuUIObj[mUICurrentNum].state != PRESSED)
		{
			mMenuUIObj[mUICurrentNum].state = SELECT;
			ConsoleWindow::Log(std::format("今のメニュー番号:{}\n", mUICurrentNum));

			if (Input::Key::Triggered(DIK_Z))
			{
				UITabMenuOn();
			}

			// 項目のナンバーを変更
			if (Input::Key::Triggered(DIK_W))
			{
				mUICurrentNum--;
				if (mUICurrentNum <= 0)
				{
					mUICurrentNum = 0;
				}
				mMenuUIObj[mUICurrentNum].state = SELECT;

				break;
			}
			if (Input::Key::Triggered(DIK_S))
			{
				mUICurrentNum++;
				if (mUICurrentNum >= mNumMenu - 1)
				{
					mUICurrentNum = mNumMenu - 1;
				}
				mMenuUIObj[mUICurrentNum].state = SELECT;
				break;
			}
		}
		else
		{
			if (Input::Key::Triggered(DIK_X))
			{
				UITabMenuOff();
			}
		}
	}

	for (size_t i = 0; i < mNumMenu; i++)
	{
		// 選択されたときとそうではないとき
		if (mMenuUIObj[i].state != PRESSED)
		{
			if (i != mUICurrentNum)
			{
				mMenuUIObj[i].state = DISABLED;
			}
		}

		switch (mMenuUIObj[i].state)
		{
		case DISABLED:
			mMenuUIObj[i].scaleChangeValue = mUIDesabledScale;
			mMenuUIObj[i].correntColor = mDesabledTextColor;
			mMenuUIObj[i].buttonColor = mDesabledColor;
			break;
		case SELECT:
			mMenuUIObj[i].scaleChangeValue = mUISelectScale;
			mMenuUIObj[i].correntColor = mSelectTextColor;
			mMenuUIObj[i].buttonColor = mSelectColor;
			break;
		case PRESSED:
			IsUITabOn = true;


			break;
		}

		mMenuUIObj[i].planeObj->scale = mMenuUIObj[i].scaleChangeValue;
		*mMenuUIObj[i].planeObj->brightnessCB.contents = mMenuUIObj[i].buttonColor.f4;
		ConsoleWindow::Log(std::format("今のメニューカラー:{},{},{},{}\n", mMenuUIObj[i].buttonColor.f4.x, mMenuUIObj[i].buttonColor.f4.y, mMenuUIObj[i].buttonColor.f4.z, mMenuUIObj[i].buttonColor.f4.w));
		mMenuUIObj[i].planeObj->Update();
	}

	// オプション項目がオンの時
	if (IsActiveOption)
	{
		mUITabEase.Update();
		mUITabBoardEase.Update();

		mTabsParentObj->rotationE.y = mUITabEase.In(DegreeToRadian(mTabRotaAfter), DegreeToRadian(mTabRotaBefore));
		mTabBoardObj->rotationE.y = mUITabBoardEase.In(DegreeToRadian(mTabBoardRotaAfter), DegreeToRadian(mTabBoardRotaBefore));

		mTabsParentObj->Update();
		mTabBoardObj->Update();

		if (mUITabBoardEase.GetisEnd())
		{
			IsActiveOption = false;
			mUITabEase.Reset();
			mUITabBoardEase.Reset();
		}
	}
}

void SuperUI::UITabMenuUpdate()
{
	// タブメニューがオンの時
	if (IsUITabOn)
	{
		mMenuTabUIObj[mCurrentTabNum].state = SELECT;

		if (Input::Key::Triggered(DIK_A))
		{
			mCurrentTabNum--;

			if (mCurrentTabNum <= 0)
			{
				mCurrentTabNum = 0;
			}

			mMenuTabUIObj[mCurrentTabNum].state = SELECT;
		}

		if (Input::Key::Triggered(DIK_D))
		{
			mCurrentTabNum++;

			if (mCurrentTabNum >= mNumOption - 1)
			{
				mCurrentTabNum = mNumOption - 1;
			}

			mMenuTabUIObj[mCurrentTabNum].state = SELECT;
		}

		for (size_t i = 0; i < mNumOption; i++)
		{
			// 選択されたときとそうではないとき

			if (i != mCurrentTabNum)
			{
				mMenuTabUIObj[i].state = DISABLED;
			}


			switch (mMenuTabUIObj[i].state)
			{
			case DISABLED:
				mMenuTabUIObj[i].scaleChangeValue = mUIDesabledScale;
				mMenuTabUIObj[i].buttonColor = mDesabledColor;
				mMenuTabUIObj[i].IsActive = false;
				break;
			case SELECT:
				mMenuTabUIObj[i].scaleChangeValue = mUISelectScale;
				mMenuTabUIObj[i].buttonColor = mSelectColor;
				mMenuTabUIObj[i].IsActive = true;
				break;
			default:
				break;
			}

			//mMenuTabUIObj[i].planeObj->scale = mMenuTabUIObj[i].scaleChangeValue;
			*mMenuTabUIObj[i].planeObj->brightnessCB.contents = mMenuTabUIObj[i].buttonColor.f4;
			mMenuTabUIObj[i].planeObj->Update();
		}
	}


}

void SuperUI::CameraMenuUpdate()
{
	//if (mUIStatus[CAMERA].state == PRESSED) 
	//{
	//	mPlayerControl = SceneManager::FindObject<PlayerControl>("PlayerControl");
	//	mCameraSensitivity = mPlayerControl->GetMouseSensitivity();
	//	if (Input::Key::Triggered(DIK_W)) 
	//	{
	//		mNumCameraItem--;

	//	}


	//}

}

void SuperUI::CameraMenuDraw()
{
	//SpDS::DrawRotaGraph(mUIStatus[CAMERA].posChangeValue.x, mUIStatus[CAMERA].posChangeValue.y,
	//	mUIStatus[CAMERA].scaleChangeValue.x, mUIStatus[CAMERA].scaleChangeValue.y, 0, "cameraTextTex",
	//	Anchor::Center, mUIStatus[CAMERA].correntColor);

	//if (mUIStatus[CAMERA].state == PRESSED) 
	//{
	//	SpDS::DrawRotaGraph(mUITextBasePos.x, mUITextBasePos.y,
	//		0.8f, 0.8f, 0, "cameraTextTex",
	//		Anchor::Center, mUIStatus[CAMERA].textCurrentColor);
	//}
}

void SuperUI::GuidMenuDraw()
{
	//SpDS::DrawRotaGraph(mUIStatus[GUID].posChangeValue.x, mUIStatus[GUID].posChangeValue.y,
	//	mUIStatus[GUID].scaleChangeValue.x, mUIStatus[GUID].scaleChangeValue.y, 0, "guidTextTex",
	//	Anchor::Center, mUIStatus[GUID].correntColor);

	//if (mUIStatus[GUID].state == PRESSED) 
	//{
	//	SpDS::DrawRotaGraph(mUITextBasePos.x, mUITextBasePos.y,
	//		0.8f, 0.8f, 0, "guidTextTex",
	//		Anchor::Center, mUIStatus[GUID].textCurrentColor);
	//}
}

void SuperUI::QuitMenuDraw()
{
	//SpDS::DrawRotaGraph(mUIStatus[QUIT_TITLE].posChangeValue.x, mUIStatus[QUIT_TITLE].posChangeValue.y,
	//	mUIStatus[QUIT_TITLE].scaleChangeValue.x, mUIStatus[QUIT_TITLE].scaleChangeValue.y, 0, "quitTextTex",
	//	Anchor::Center, mUIStatus[QUIT_TITLE].correntColor);

	//if (mUIStatus[QUIT_TITLE].state == PRESSED) 
	//{
	//	SpDS::DrawRotaGraph(mUITextBasePos.x, mUITextBasePos.y,
	//		0.8f, 0.8f, 0, "quitTextTex",
	//		Anchor::Center, mUIStatus[QUIT_TITLE].textCurrentColor);
	//}
}

void SuperUI::UIMainMenuOnReset()
{
	mMenuParentObj->Activate();

	// メインメニュー関連をオン
	mMenuPlaneObj->Activate();
	mPlanesParentObj->Activate();
	mUICurrentNum = 0;
	mMenuUIObj[mUICurrentNum].state = SELECT;

	// タブをオフ
	mTabsParentObj->Deactivate();

	// タブ番号をリセット
	mCurrentTabNum = 0;

	IsUITabOn = false;
	IsActiveOption = false;

	mUITabEase.Reset();
	mUITabBoardEase.Reset();
}

void SuperUI::UIMainMenuOffReset()
{
	mMenuParentObj->Deactivate();
}

void SuperUI::UITabMenuOn()
{
	mMenuUIObj[mUICurrentNum].state = PRESSED;
	IsActiveOption = true;

	// メインメニュー関連をオフ
	mMenuPlaneObj->Deactivate();
	mPlanesParentObj->Deactivate();

	// タブをオン
	mTabsParentObj->Activate();

	mCurrentTabNum = 0;

	mTabItems[mCurrentTabNum].OnUpdate();
}

void SuperUI::UITabMenuOff()
{
	mTabItems[mCurrentTabNum].OffUpdate();

	mUIBoardCurrentColor = mDesabledColor;
	mMenuUIObj[mUICurrentNum].state = SELECT;
	IsUITabOn = false;
	mCurrentTabNum = 0;
	mMenuUIObj[mUICurrentNum].textCurrentColor = mUITextBeforeColor;

	// メインメニュー関連をオフ
	mMenuPlaneObj->Activate();
	mPlanesParentObj->Activate();

	// タブをオフ
	mTabsParentObj->Deactivate();
}

RegisterScriptBody(SuperUI);
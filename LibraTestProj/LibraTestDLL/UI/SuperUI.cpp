#include "SuperUI.h"
#include <Camera.h>
#include <SpDS.h>
#include <SpTextureManager.h>
#include <Util.h>
#include <SceneManager.h>
#include <Input.h>
#include <ConsoleWindow.h>
#include "GameManager.h"
#include "PlayerControl.h"

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
	// もしTABキーを押したらメニューを開け閉めする
	if (Input::Key::Triggered(DIK_ESCAPE))
	{
		ConsoleWindow::Log("TAB押された。");
		if (mIsDisplayUI)
		{
			SceneManager::FindObject<GameManager>("GameManager")->SetIsStop(false);
			mIsDisplayUI = false;
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

	if (mIsDisplayUI)
	{
		UIObj3DUpdate();
		ConsoleWindow::Log("メニューを開いた");
	}

}

void SuperUI::Draw()
{

}

void SuperUI::CopyComponent(IComponent* src)
{

}

bool SuperUI::GetBackToTitle()
{
	return IsBackToTitle;
}

void SuperUI::UIObj3DInit()
{

	// 項目数の初期化
	mNumMenu = 3;

	mNumOption = 3;

	mTabRotaAfter = -540;

	mTabRotaBefore = 0;

	mTabBoardRotaAfter = -545;

	mTabBoardRotaBefore = 0;

	mTabEaseTimeLimit = 30;

	IsUITabOn = false;

	IsGuidOn = false;

	IsQuitTitleOn = false;

	IsBackToTitle = false;

	mCurrentTabNum = 0;

	mQuitTitleSelectScale = { 0.16f,0.06f,1 };
	mQuitTitleDisabledScale = { 0.15f,0.05f,1 };

	mUITabEase.SetEaseTimer((int32_t)mTabEaseTimeLimit);

	mUITabBoardEase.SetEaseTimer((int32_t)(mTabEaseTimeLimit * 1.5f));

	// メニューのリストをリサイズ
	mMenuUIObj.resize(mNumMenu);
	mMenuTabUIObj.resize(mNumOption);
	mTabItems.resize(mNumOption);
	

	// シーンに配置されているオブジェクトを走査して、代入

	//mCameraItem.Init();
	//mGpraphicsItem.Init();
	//mSoundItem.Init();
	mCameraItem = SceneManager::FindObject<CameraTab>("CameraTabScript"); 
	mGpraphicsItem = SceneManager::FindObject<GraphicsTab>("GraphicsTabScript");
	mSoundItem = SceneManager::FindObject<SoundTab>("SoundTabScript");

	mTabItems[CAMERA] = mCameraItem;
	mTabItems[GRAPHICS] = mGpraphicsItem;
	mTabItems[SOUND] = mSoundItem;

	for (size_t i = 0; i < mNumOption; i++)
	{
		mTabItems[i]->Init();
	}

	mMainCameraObj = SceneManager::FindObject<Object3D>("Camera");

	mMenuParentObj = SceneManager::FindObject<Object3D>("UIParentObj");
	mMenuParentObj->parent = mMainCameraObj;
	mMenuParentObj->Deactivate();

	mTabsParentObj=SceneManager::FindObject<Object3D>("Tabs");
	mTabBoardObj=SceneManager::FindObject<Object3D>("Board");
	mMenuPlaneObj = SceneManager::FindObject<Object3D>("MenuPlane");
	mPlanesParentObj = SceneManager::FindObject<Object3D>("Planes");

	// メインメニューの項目のオブジェクト設定
	mMenuUIObj[GUID].planeObj = SceneManager::FindObject<Object3D>("GuidPlane");
	mMenuUIObj[OPTIONS].planeObj = SceneManager::FindObject<Object3D>("OptionPlane");
	mMenuUIObj[QUIT_TITLE].planeObj = SceneManager::FindObject<Object3D>("QuitTitlePlane");

	// オプションメニューの項目のオブジェクト設定
	mMenuTabUIObj[CAMERA].planeObj = SceneManager::FindObject<Object3D>("CameraTab");
	mMenuTabUIObj[GRAPHICS].planeObj = SceneManager::FindObject<Object3D>("GraphicsTab");
	mMenuTabUIObj[SOUND].planeObj = SceneManager::FindObject<Object3D>("SoundTab");

	// ガイドメニューのオブジェクト設定
	mGuidParentObj = SceneManager::FindObject<Object3D>("GuidMenu");

	// タイトルへ戻るオブジェクト設定
	mQuitTitleParentObj = SceneManager::FindObject<Object3D>("QuitTitleMenu");
	mQuitTextObjs.resize(2);
	mQuitTextObjs[Yes].planeObj = SceneManager::FindObject<Object3D>("YesText");
	mQuitTextObjs[No].planeObj = SceneManager::FindObject<Object3D>("NoText");

}

void SuperUI::UIObj3DUpdate()
{

	UIMainMenuUpdate();

	UITabMenuUpdate();

	UITitleMenuUpdate();
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
				switch (mUICurrentNum)
				{
				case GUID:
					UIGuidMenuOn();
					break;
				case OPTIONS:
					UITabMenuOn();
					break;
				case QUIT_TITLE:
					UIQuitTitleMenuOn();
					break;
				}
				
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
				switch (mUICurrentNum)
				{
				case GUID:
					UIGuidMenuOff();
					break;
				case OPTIONS:
					UITabMenuOff();
					break;
				case QUIT_TITLE:
					UIQuitTitleMenuOff();
					break;
				}
				
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
			switch (mUICurrentNum)
			{
			case GUID:
				IsGuidOn = true;
				break;
			case OPTIONS:
				IsUITabOn = true;
				break;
			case QUIT_TITLE:
				IsQuitTitleOn = true;
				break;
			}



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
		mTabItems[mCurrentTabNum]->MenuUpdate();
		if (Input::Key::Triggered(DIK_Q))
		{
			mCurrentTabNum--;

			if (mCurrentTabNum <= 0)
			{
				mCurrentTabNum = 0;
			}

			mMenuTabUIObj[mCurrentTabNum].state = SELECT;
			mTabItems[mCurrentTabNum]->OnUpdate();
		}

		if (Input::Key::Triggered(DIK_E))
		{
			mCurrentTabNum++;

			if (mCurrentTabNum >= mNumOption - 1)
			{
				mCurrentTabNum = mNumOption - 1;
			}

			mMenuTabUIObj[mCurrentTabNum].state = SELECT;
			mTabItems[mCurrentTabNum]->OnUpdate();
		}

		for (size_t i = 0; i < mNumOption; i++)
		{
			// 選択されたときとそうではないとき

			if (i != mCurrentTabNum)
			{
				mMenuTabUIObj[i].state = DISABLED;
				mTabItems[i]->OffUpdate();
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

void SuperUI::UITitleMenuUpdate()
{
	if (IsQuitTitleOn)
	{
		if (Input::Key::Triggered(DIK_A))
		{
			mQuitTextObjs[Yes].planeObj->scale = mQuitTitleSelectScale;
			mQuitTextObjs[No].planeObj->scale = mQuitTitleDisabledScale;

			mQuitTextObjs[Yes].state = SELECT;
			mQuitTextObjs[No].state = DISABLED;
		}
		if (Input::Key::Triggered(DIK_D))
		{
			mQuitTextObjs[Yes].planeObj->scale = mQuitTitleDisabledScale;
			mQuitTextObjs[No].planeObj->scale = mQuitTitleSelectScale;

			mQuitTextObjs[Yes].state = DISABLED;
			mQuitTextObjs[No].state = SELECT;
		}

		if (mQuitTextObjs[Yes].state == SELECT)
		{
			if (Input::Key::Triggered(DIK_Z))
			{
				IsBackToTitle = true;
			}
		}
		if (mQuitTextObjs[No].state == SELECT)
		{
			if (Input::Key::Triggered(DIK_Z))
			{
				IsBackToTitle = false;
				UIQuitTitleMenuOff();
			}
		}

		for (size_t i = 0; i < 2; i++)
		{
			mQuitTextObjs[i].planeObj->Update();
		}
		
		
	}
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
	mGuidParentObj->Deactivate();
	mQuitTitleParentObj->Deactivate();

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

	for (size_t i = 0; i < mNumMenu; i++)
	{
		mMenuUIObj[i].state = DISABLED;
	}
}

void SuperUI::UITabMenuOn()
{
	mMenuUIObj[mUICurrentNum].state = PRESSED;
	IsActiveOption = true;

	// メインメニュー関連をオフ
	mMenuPlaneObj->Deactivate();
	mPlanesParentObj->Deactivate();
	mUITabEase.Reset();
	mUITabBoardEase.Reset();

	// タブをオン
	mTabsParentObj->Activate();

	mCurrentTabNum = 0;

	mTabItems[mCurrentTabNum]->OnUpdate();
}

void SuperUI::UITabMenuOff()
{
	mTabItems[mCurrentTabNum]->OffUpdate();

	mUIBoardCurrentColor = mDesabledColor;
	mMenuUIObj[mUICurrentNum].state = SELECT;
	IsUITabOn = false;
	mCurrentTabNum = 0;
	mMenuUIObj[mUICurrentNum].textCurrentColor = mUITextBeforeColor;
	mUITabEase.Reset();
	mUITabBoardEase.Reset();

	// メインメニュー関連をオフ
	mMenuPlaneObj->Activate();
	mPlanesParentObj->Activate();


	// タブをオフ
	mTabsParentObj->Deactivate();
}

void SuperUI::UIGuidMenuOn()
{
	mMenuUIObj[mUICurrentNum].state = PRESSED;

	// メインメニュー関連をオフ
	mMenuPlaneObj->Deactivate();
	mPlanesParentObj->Deactivate();

	mGuidParentObj->Activate();

}

void SuperUI::UIGuidMenuOff()
{
	mMenuUIObj[mUICurrentNum].state = SELECT;

	// メインメニュー関連をオン
	mMenuPlaneObj->Activate();
	mPlanesParentObj->Activate();

	mGuidParentObj->Deactivate();
}

void SuperUI::UIQuitTitleMenuOn()
{
	mMenuUIObj[mUICurrentNum].state = PRESSED;

	// メインメニュー関連をオフ
	mMenuPlaneObj->Deactivate();
	mPlanesParentObj->Deactivate();


	mQuitTitleParentObj->Activate();
	mQuitTextObjs[Yes].planeObj->scale = mQuitTitleSelectScale;
	mQuitTextObjs[No].planeObj->scale = mQuitTitleDisabledScale;
	mQuitTextObjs[Yes].state = SELECT;
	mQuitTextObjs[No].state = DISABLED;
}

void SuperUI::UIQuitTitleMenuOff()
{
	mMenuUIObj[mUICurrentNum].state = SELECT;
	IsQuitTitleOn = false;

	// メインメニュー関連をオン
	mMenuPlaneObj->Activate();
	mPlanesParentObj->Activate();

	mQuitTitleParentObj->Deactivate();
	mQuitTextObjs[Yes].state = SELECT;
	mQuitTextObjs[No].state = DISABLED;
}

RegisterScriptBody(SuperUI);
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
#include "AppOperationCommand.h"

float DegreeToRadian(float angle)
{
	float PIPI = 3.141592f;
	float radian = angle * PIPI / 180;

	return radian;
}

// 配列の要素を右にシフトする関数
void shiftRight(std::vector<Object3D*> vec) {
	if (vec.empty()) return;
	Object3D* last = vec.back();
	for (size_t i = 1; i < vec.size(); i++) {
		vec[i] = vec[i - 1];
	}
	vec[0] = last;
}

// 配列の要素を左にシフトする関数
void shiftLeft(std::vector<Object3D*> vec) {
	if (vec.empty()) return;
	Object3D* first = vec.front();
	for (size_t i = 0; i < vec.size() - 1; i++) {
		vec[i] = vec[i + 1];
	}
	vec.back() = first;
}

void SuperUI::Init()
{

	//testObj = SceneManager::FindObject<Object3D>("Object3D");

	// フラグ関連の初期化
	mIsOpenUIMenu = false;

	// テクスチャのロード
	//LoadTexInit();

	mUICurrentNum = 0;

	mUIDesabledScale = { 0.4f ,0.1f };
	mUISelectScale = { 0.5f ,0.12f };

	mDesabledColor = { 180,180,180 };
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
	if (!mCameraItem || !mSoundItem ||
		!mMainCameraObj || !mMenuParentObj || !mTabsParentObj ||
		!mMenuPlaneObj || !mPlanesParentObj ||
		!mGuidParentObj || !mQuitTitleParentObj || !mTabsPParentObj || !mTabBoardParentObj)
	{
		return;
	}

	for (uint32_t i = 0; i < mMenuUIObj.size(); i++)
	{
		if (!mMenuUIObj[i].planeObj)
		{
			return;
		}
	}

	for (uint32_t i = 0; i < mMenuTabUIObj.size(); i++)
	{
		if (!mMenuTabUIObj[i].planeObj)
		{
			return;
		}
	}

	for (uint32_t i = 0; i < mQuitTextObjs.size(); i++)
	{
		if (!mQuitTextObjs[i].planeObj)
		{
			return;
		}
	}

	for (size_t i = 0; i < mMainMenuObjs.size(); i++)
	{
		if (!mMainMenuObjs[i])
		{
			return;
		}
	}

	for (size_t i = 0; i < mTabBoardObjs.size(); i++)
	{
		if (!mTabBoardObjs[i])
		{
			return;
		}
	}

	// もしTABキーを押したらメニューを開け閉めする
	if (AppOperationCommand::GetInstance()->UIBackCommand())
	{
		OutputDebugStringA("menuOpen");
		if ((mIsDisplayUI || mIsMomentOpenMenu)&& 
			mMenuUIObj[mUICurrentNum].state != PRESSED)
		{
			SceneManager::FindObject<GameManager>("GameManager")->SetIsStop(false);
			mIsDisplayUI = false;
			UIMainMenuOffReset();
			ConsoleWindow::Log("メニューを閉じた");
		}
		else if (mMenuUIObj[mUICurrentNum].state != PRESSED)
		{
			SceneManager::FindObject<GameManager>("GameManager")->SetIsStop(true);
			mIsMomentOpenMenu = true;
			UIMainMenuOnReset();

			ConsoleWindow::Log("メニューを開いた");
		}
	}
	if (mIsMomentOpenMenu)
	{
		UIMainMenuMomentUpdate();
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

	mNumOption = 2;

	mTabRotaAfter = 30;

	mTabRotaBefore = 0;

	mTabBoardRotaAfter = -95;

	mTabBoardRotaBefore = 0;

	mTabEaseTimeLimit = 30;

	IsUITabOn = false;

	IsGuidOn = false;

	IsQuitTitleOn = false;

	IsBackToTitle = false;

	mCurrentTabNum = 0;

	mQuitTitleSelectScale = { 0.1f,0.06f,1 };
	mQuitTitleDisabledScale = { 0.09f,0.05f,1 };

	mUITabEase.SetEaseTimer((int32_t)mTabEaseTimeLimit);

	mUITabBoardEase.SetEaseTimer((int32_t)(mTabEaseTimeLimit));

	mUITabAlphaEase.SetEaseTimer((int32_t)60);


	mUITabEase.SetPowNum(5);
	mUITabBoardEase.SetPowNum(5);
	mUITabAlphaEase.SetPowNum(5);

	// メニューのリストをリサイズ
	mMenuUIObj.resize(mNumMenu);
	mMenuTabUIObj.resize(mNumOption);
	mTabItems.resize(mNumOption);


	// シーンに配置されているオブジェクトを走査して、代入

	//mCameraItem.Init();
	//mGpraphicsItem.Init();
	//mSoundItem.Init();
	mCameraItem = SceneManager::FindObject<CameraTab>("CameraTabScript");
	mSoundItem = SceneManager::FindObject<SoundTab>("SoundTabScript");

	mTabItems[CAMERA] = mCameraItem;
	mTabItems[SOUND] = mSoundItem;

	for (size_t i = 0; i < mNumOption; i++)
	{
		if (!mTabItems[i])
		{
			continue;
		}

		mTabItems[i]->Init();
	}

	mMainCameraObj = SceneManager::FindObject<Object3D>("Camera");
	if (!mMainCameraObj)
	{
		return;
	}

	mMenuParentObj = SceneManager::FindObject<Object3D>("UIParentObj");
	if (!mMenuParentObj)
	{
		return;
	}
	mMenuParentObj->parent = mMainCameraObj;
	mMenuParentObj->Deactivate();

	mTabsParentObj=SceneManager::FindObject<Object3D>("Tabs");

	mMenuPlaneObj = SceneManager::FindObject<Object3D>("MenuPlane");
	mPlanesParentObj = SceneManager::FindObject<Object3D>("Planes");

	// メインメニューの項目のオブジェクト設定
	mMenuUIObj[GUID].planeObj = SceneManager::FindObject<Object3D>("GuidPlane");
	mMenuUIObj[OPTIONS].planeObj = SceneManager::FindObject<Object3D>("OptionPlane");
	mMenuUIObj[QUIT_TITLE].planeObj = SceneManager::FindObject<Object3D>("QuitTitlePlane");

	// オプションメニューの項目のオブジェクト設定
	mMenuTabUIObj[CAMERA].planeObj = SceneManager::FindObject<Object3D>("CameraTab");
	mMenuTabUIObj[SOUND].planeObj = SceneManager::FindObject<Object3D>("SoundTab");

	mMenuTabTextObj.push_back(SceneManager::FindObject<Object3D>("CameraTabText"));
	mMenuTabTextObj.push_back(SceneManager::FindObject<Object3D>("SoundTabText"));

	// ガイドメニューのオブジェクト設定
	mGuidParentObj = SceneManager::FindObject<Object3D>("GuidMenu");

	// タイトルへ戻るオブジェクト設定
	mQuitTitleParentObj = SceneManager::FindObject<Object3D>("QuitTitleMenu");
	mQuitTextObjs.resize(2);
	mQuitTextObjs[Yes].planeObj = SceneManager::FindObject<Object3D>("YesText");
	mQuitTextObjs[No].planeObj = SceneManager::FindObject<Object3D>("NoText");

	mTabsPParentObj = SceneManager::FindObject<Object3D>("TabsParent");
	mTabBoardParentObj= SceneManager::FindObject<Object3D>("BoardParent");
	
	mTabBoardObjs.push_back(SceneManager::FindObject<Object3D>("CameraBoardParent"));
	mTabBoardObjs.push_back(SceneManager::FindObject<Object3D>("SoundBoardParent"));

	mMainMenuObjs.push_back(SceneManager::FindObject<Object3D>("BackBlack"));
	mMainMenuObjs.push_back(SceneManager::FindObject<Object3D>("MenuPlane"));
	mMainMenuObjs.push_back(SceneManager::FindObject<Object3D>("MenuText"));
	mMainMenuObjs.push_back(SceneManager::FindObject<Object3D>("GuidPlane"));
	mMainMenuObjs.push_back(SceneManager::FindObject<Object3D>("GuidText"));
	mMainMenuObjs.push_back(SceneManager::FindObject<Object3D>("OptionPlane"));
	mMainMenuObjs.push_back(SceneManager::FindObject<Object3D>("OptionText"));
	mMainMenuObjs.push_back(SceneManager::FindObject<Object3D>("QuitTitlePlane"));
	mMainMenuObjs.push_back(SceneManager::FindObject<Object3D>("QuitTitleText"));

	mGuideParent = SceneManager::FindObject<Object3D>("GuidMenu");
	mBackParent = SceneManager::FindObject<Object3D>("QuitTitleMenu");

	mMainAlphaEase.SetEaseTimer(20);
	mBackBlackAlpha = { 0,0,0,150 };
	mMainMenuColor = { 0,0,0,255 };

	mTabChangeEase.SetEaseTimer(20);
	mTabChangeEase.SetPowNum(5);

	// 回転用のベクター
	mTabBoardObjs[0]->rotationE.y = DegreeToRadian(0);
	mTabBoardObjs[1]->rotationE.y = DegreeToRadian(240);

	for (size_t i = 0; i < mNumOption; i++)
	{
		mTabBoardObjs[i]->Update();
	}

	mQuitTimer.SetLimitTimer(60 * 2);

	mGuideKeyTexObj = SceneManager::FindObject<Object3D>("KeyGuidTex");
	mGuidePadTexObj = SceneManager::FindObject<Object3D>("PadGuidTex");

	mGuideEase.SetEaseTimer(20);
	mGuideEase.SetPowNum(5);
	mBackTitleEase.SetEaseTimer(20);
	mBackTitleEase.SetPowNum(5);
}

void SuperUI::UIObj3DUpdate()
{
	UIMainMenuUpdate();

	UITabMenuUpdate();

	UIGuideMenuUpdate();

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

void SuperUI::UIMainMenuMomentUpdate()
{
	OutputDebugStringA("UIMomentUpdate---");

	mMainAlphaEase.Update();

	Color backColor;
	backColor.f4.w = mMainAlphaEase.In(0, mBackBlackAlpha.f4.w);
	Color menuColor;
	menuColor.f4.w = mMainAlphaEase.In(0, mMainMenuColor.f4.w);

	mMainMenuObjs[BackBlack]->brightnessCB.contents->w = backColor.f4.w;

	for (size_t i = 0; i < mMainMenuObjs.size(); i++)
	{
		if (i != BackBlack)
		{
			mMainMenuObjs[i]->brightnessCB.contents->w = menuColor.f4.w;
		}
		mMainMenuObjs[i]->Update();
	}

	if (mMainAlphaEase.GetisEnd())
	{
		mMainAlphaEase.Reset();
		mIsMomentOpenMenu = false;
		mIsDisplayUI = true;
	}
}

void SuperUI::UIMainMenuUpdate()
{
	for (size_t i = 0; i < mNumMenu; i++)
	{		// 選択されたときとそうではないとき
		if (mMenuUIObj[mUICurrentNum].state != PRESSED)
		{
			mMenuUIObj[mUICurrentNum].state = SELECT;
			ConsoleWindow::Log(std::format("今のメニュー番号:{}\n", mUICurrentNum));

			if (AppOperationCommand::GetInstance()->UIDicisionCommand())
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
			if (AppOperationCommand::GetInstance()->UISelectUpCommand())
			{
				mUICurrentNum--;
				if (mUICurrentNum <= 0)
				{
					mUICurrentNum = 0;
				}
				mMenuUIObj[mUICurrentNum].state = SELECT;

				break;
			}
			if (AppOperationCommand::GetInstance()->UISelectDownCommand())
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
			if (AppOperationCommand::GetInstance()->UIBackCommand())
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
		mUITabAlphaEase.Update();

		for (size_t i = 0; i < mNumOption; i++)
		{
			Color alphaColor,alphaColorRed;
			alphaColorRed = mDesabledColor;
			alphaColor.f4 = { 1, 1, 1, mUITabAlphaEase.Out(0, 1) };
			alphaColorRed.f4.w = mUITabAlphaEase.Out(0, 1);

			*mMenuTabUIObj[CAMERA].planeObj->brightnessCB.contents = alphaColor;
			*mMenuTabUIObj[SOUND].planeObj->brightnessCB.contents = alphaColorRed;
			*mMenuTabTextObj[CAMERA]->brightnessCB.contents = alphaColor;
			*mMenuTabTextObj[SOUND]->brightnessCB.contents = alphaColorRed;
			mMenuTabUIObj[i].planeObj->Update();
		}

		mTabsPParentObj->rotationE.y = mUITabEase.Out(DegreeToRadian(mTabRotaAfter), DegreeToRadian(mTabRotaBefore));
		mTabBoardParentObj->rotationE.y = mUITabBoardEase.Out(DegreeToRadian(mTabBoardRotaAfter), DegreeToRadian(mTabBoardRotaBefore));

		mTabsPParentObj->Update();
		mTabBoardParentObj->Update();

		if (mUITabAlphaEase.GetisEnd())
		{
			IsUITabOn = true;
			IsActiveOption = false;
			mUITabEase.Reset();
			mUITabBoardEase.Reset();
			mUITabAlphaEase.Reset();
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


		// タブの回転処理
		if (mIsTabChange)
		{
			mTabChangeEase.Update();

			if (mIsTabRight)
			{
				if (mIsTabSet)
				{
					mTabRotaFirst = mTabBoardParentObj->rotationE.y;
					mTabBoardParentRotaAfter = mTabRotaFirst + DegreeToRadian(120);
					mIsTabSet = false;

					if (mTabRotaFirst >= DegreeToRadian(120))
					{
						mTabBoardParentRotaAfter = 120;
						mTabChangeEase.Reset();
						mIsTabRight = false;
						mIsTabChange = false;
					}
				}
				mTabBoardParentObj->rotationE.y = mTabChangeEase.Out(mTabRotaFirst, mTabBoardParentRotaAfter);
				

				if (mTabChangeEase.GetisEnd())
				{
					mTabChangeEase.Reset();
					mIsTabRight = false;
					mIsTabChange = false;
				}
			}
			else if (mIsTabLeft)
			{

				if (mIsTabSet)
				{
					mTabRotaFirst = mTabBoardParentObj->rotationE.y;
					mTabBoardParentRotaAfter = mTabRotaFirst - DegreeToRadian(120);
					mIsTabSet = false;

					if (mTabRotaFirst <= 0)
					{
						mTabBoardParentRotaAfter = 0;
						mTabChangeEase.Reset();
						mIsTabRight = false;
						mIsTabChange = false;
					}
				}

				mTabBoardParentObj->rotationE.y = mTabChangeEase.Out(mTabRotaFirst, mTabBoardParentRotaAfter);

				if (mTabChangeEase.GetisEnd())
				{
					mTabChangeEase.Reset();
					mIsTabLeft = false;
					mIsTabChange = false;
				}
			}

			for (size_t i = 0; i < mNumOption; i++)
			{
				mTabBoardObjs[i]->Update();
			}
		}
		else 
		{
			if (AppOperationCommand::GetInstance()->UITabLeftCommand())
			{
				mCurrentTabNum--;

				if (mCurrentTabNum <= 0)
				{
					mCurrentTabNum = 0;
				}
				mIsTabSet = true;
				mIsTabLeft = true;
				mIsTabChange = true;

				mMenuTabUIObj[mCurrentTabNum].state = SELECT;
				mTabItems[mCurrentTabNum]->OnUpdate();
			}

			if (AppOperationCommand::GetInstance()->UITabRightCommand())
			{
				mCurrentTabNum++;

				if (mCurrentTabNum >= mNumOption - 1)
				{
					mCurrentTabNum = mNumOption - 1;
				}
				mIsTabSet = true;
				mIsTabRight= true;
				mIsTabChange = true;

				mMenuTabUIObj[mCurrentTabNum].state = SELECT;
				mTabItems[mCurrentTabNum]->OnUpdate();
			}
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

				*mMenuTabTextObj[i]->brightnessCB.contents = mDesabledColor;

				break;
			case SELECT:
				mMenuTabUIObj[i].scaleChangeValue = mUISelectScale;
				mMenuTabUIObj[i].buttonColor = mSelectColor;
				mMenuTabUIObj[i].IsActive = true;

				*mMenuTabTextObj[i]->brightnessCB.contents = mSelectColor;

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

void SuperUI::UIGuideMenuUpdate()
{
	if (IsGuidOn)
	{
		mGuideEase.Update();

		mGuideParent->rotationE.y = mGuideEase.Out(DegreeToRadian(mTabBoardRotaAfter), DegreeToRadian(mTabBoardRotaBefore));
		mGuideParent->Update();
	}
}

void SuperUI::UITitleMenuUpdate()
{
	if (IsQuitTitleOn)
	{
		mBackTitleEase.Update();
		mBackParent->rotationE.y = mBackTitleEase.Out(DegreeToRadian(mTabBoardRotaAfter), DegreeToRadian(mTabBoardRotaBefore));
		mBackParent->Update();

		if (mBackTitleEase.GetisEnd())
		{
			mQuitTimer.Update();

			if (mQuitTimer.GetisTimeOut())
			{
				if (AppOperationCommand::GetInstance()->UISelectLeftCommand())
				{
					mQuitTextObjs[Yes].planeObj->scale = mQuitTitleSelectScale;
					mQuitTextObjs[No].planeObj->scale = mQuitTitleDisabledScale;

					mQuitTextObjs[Yes].state = SELECT;
					mQuitTextObjs[No].state = DISABLED;
				}
				if (AppOperationCommand::GetInstance()->UISelectRightCommand())
				{
					mQuitTextObjs[Yes].planeObj->scale = mQuitTitleDisabledScale;
					mQuitTextObjs[No].planeObj->scale = mQuitTitleSelectScale;

					mQuitTextObjs[Yes].state = DISABLED;
					mQuitTextObjs[No].state = SELECT;
				}

				if (mQuitTextObjs[Yes].state == SELECT)
				{
					if (AppOperationCommand::GetInstance()->UIDicisionCommand())
					{
						IsBackToTitle = true;
					}
				}
				if (mQuitTextObjs[No].state == SELECT)
				{
					if (AppOperationCommand::GetInstance()->UIDicisionCommand())
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
	mUITabAlphaEase.Reset();
	mMainAlphaEase.Reset();

	mTabChangeEase.Reset();
	mIsTabLeft = false;
	mIsTabRight = false;
	mIsTabChange = false;

	UIMainMenuUpdate();
}

void SuperUI::UIMainMenuOffReset()
{
	mMenuParentObj->Deactivate();
	mIsDisplayUI = false;
	mIsMomentOpenMenu = false;
	mMainAlphaEase.Reset();

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
	mUITabAlphaEase.Reset();
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
	mUITabAlphaEase.Reset();
	// メインメニュー関連をオフ
	mMenuPlaneObj->Activate();
	mPlanesParentObj->Activate();

	// タブメニューの回転関連
	mTabChangeEase.Reset();
	mIsTabLeft = false;
	mIsTabRight = false;
	mIsTabChange = false;

	mTabBoardObjs[0]->rotationE.y = DegreeToRadian(0);
	mTabBoardObjs[1]->rotationE.y = DegreeToRadian(240);

	for (size_t i = 0; i < mNumOption; i++)
	{
		mTabBoardObjs[i]->Update();
	}

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

	if (Input::Pad::IsConnected())
	{
		mGuideKeyTexObj->Deactivate();
		mGuidePadTexObj->Activate();
	}
	else {
		mGuideKeyTexObj->Activate();
		mGuidePadTexObj->Deactivate();
	}

	mGuideEase.Reset();
}

void SuperUI::UIGuidMenuOff()
{
	mMenuUIObj[mUICurrentNum].state = SELECT;
	IsGuidOn = false;

	// メインメニュー関連をオン
	mMenuPlaneObj->Activate();
	mPlanesParentObj->Activate();

	mGuidParentObj->Deactivate();
	mGuideEase.Reset();
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
	mQuitTimer.Reset();

	mBackTitleEase.Reset();
}

void SuperUI::UIQuitTitleMenuOff()
{
	mMenuUIObj[mUICurrentNum].state = SELECT;
	IsQuitTitleOn = false;

	// メインメニュー関連をオン
	mMenuPlaneObj->Activate();
	mPlanesParentObj->Activate();

	mQuitTitleParentObj->Deactivate();
	mQuitTextObjs[Yes].state = DISABLED;
	mQuitTextObjs[No].state = SELECT;
	mQuitTimer.Reset();

	mBackTitleEase.Reset();
}

RegisterScriptBody(SuperUI);
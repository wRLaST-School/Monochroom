#include "SelectPanel.h"
#include <ScriptComponent.h>
#include <SpDS.h>
#include <Util.h>
#include <Input.h>
#include <SceneManager.h>
#include <ConsoleWindow.h>
#include <SpTextureManager.h>

Vec3 DegreeToRadian(Vec3& angle)
{

	Vec3 radian = angle * (float)(PI / 180);

	return radian;
}

void SelectPanel::Init()
{
	mSelectMaxNumY = 2;
	mSelectMaxNumX.resize(mSelectMaxNumY);

	mSelectMaxNumX[Stage1] = 3;
	mSelectMaxNumX[Stage2] = 4;

	mSelectCurrentNum = { 0,0 };

	mStageNum.resize(mSelectMaxNumY);

	mDisabledPanelScale = { 1,1,1 };
	mSelectPanelScale = { 1.1f,1.1f,1 };
	mPressedPanelScale = { 0.9f,0.9f,0.9f };

	mNumObjMax = 10;

	mNumberTex.resize(mNumObjMax);

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
	mHyphenTex = "hyphen";

	mNumTexSize = { 320,320 };

	mTitleCameraFirstPos = Vec3(-4, 4.68f, 2);
	mTitleCameraFirstRota = { 0, -10, 0 };
	mTitleCameraFirstRota = DegreeToRadian(mTitleCameraFirstRota);

	mSceneChangeCameraFirstPos = Vec3(1.6f, 1.1f, -2.73f);
	mSceneChangeCameraFirstRota = Vec3(65, 0, 0);
	mSceneChangeCameraFirstRota= DegreeToRadian(mSceneChangeCameraFirstRota);

	// カメラのセット
	mCameraObj = SceneManager::FindObject<Object3D>("Camera");

	mCameraObj->position = mTitleCameraFirstPos;
	mCameraObj->rotationE = mTitleCameraFirstRota;
	mCameraObj->Update();

	// テクスチャ切り分け
	SpTextureManager::LoadDiv("Assets/Images/numbers1.png", (int)mNumTexSize.x, (int)mNumTexSize.y, 10, 1, mNumberTex);

	SpTextureManager::LoadTexture("Assets/Images/hyphen.png", mHyphenTex);

	for (size_t i = 0; i < mSelectMaxNumY; i++)
	{
		mStageNum[i].resize(mSelectMaxNumX[i]);
	}

	for (size_t i = 0; i < mSelectMaxNumY; i++)
	{
		for (size_t j = 0; j < mSelectMaxNumX[i]; j++)
		{
			std::string stageFirstName = "Stage";
			std::string stageBaseNum = std::to_string(i + 1) + "-" + std::to_string(j + 1);

			std::string stageName = stageFirstName + stageBaseNum;

			mStageNum[i][j].buttonObj = SceneManager::FindObject<Object3D>(stageName);
			mStageNum[i][j].state = DISABLED;

			// ステージ番号の板ポリの初期化
			mStageNum[i][j].stageBaseNumObj = SceneManager::FindChildObject<Object3D>("StageNum", mStageNum[i][j].buttonObj);
			mStageNum[i][j].stageSubNumObj = SceneManager::FindChildObject<Object3D>("SubNum", mStageNum[i][j].buttonObj);
			mStageNum[i][j].hyphenObj = SceneManager::FindChildObject<Object3D>("Hyphen", mStageNum[i][j].buttonObj);

			mStageNum[i][j].stageBaseNumObj->texture = mNumberTex[i + 1];
			mStageNum[i][j].stageSubNumObj->texture = mNumberTex[j + 1];
			mStageNum[i][j].hyphenObj->texture = mHyphenTex;
		}
	}

	// タイトルのカメラ配列設定
	mTitleCameraMovePos.push_back(mTitleCameraFirstPos);
	mTitleCameraMovePos.push_back(Vec3(0, 2, -5));
	mTitleCameraMovePos.push_back(Vec3(1, 2, -4));
	mTitleCameraMovePos.push_back(mSceneChangeCameraFirstPos);

	mTitleCameraMoveRota.push_back(Vec3( 0, -10, 0 ));
	mTitleCameraMoveRota.push_back(Vec3(0, 0, 0));
	mTitleCameraMoveRota.push_back(Vec3(25, 15, 0));
	mTitleCameraMoveRota.push_back(Vec3(65, 0, 0));

	// シーンチェンジのカメラ配列設定
	mSceneChangeCameraMovePos.push_back(mSceneChangeCameraFirstPos);
	mSceneChangeCameraMovePos.push_back(Vec3(1, 2, -4.5f));
	mSceneChangeCameraMovePos.push_back(Vec3(0, 1.8f, -2.5f));
	mSceneChangeCameraMovePos.push_back(Vec3(0, 2, -0.1f));

	mSceneChangeCameraMoveRota.push_back(Vec3(65, 0, 0));
	mSceneChangeCameraMoveRota.push_back(Vec3(30, -60, 0));
	mSceneChangeCameraMoveRota.push_back(Vec3(15, -10, 0));
	mSceneChangeCameraMoveRota.push_back(Vec3(10, 0, 0));

	mSelectState = TITLE;

	mTitleMoveTime = 0;
	mTitleMoveTimeMax = 60 * 6;

	mSceneChangeCameraTime = 0;
	mSceneChangeCameraTimeMax = 60 * 5;
}

void SelectPanel::Update()
{
	switch (mSelectState)
	{
	case SelectPanel::TITLE:
		TitleUpdate();
		break;
	case SelectPanel::SELECTSTAGE:
		SelectStageUpdate();
		break;
	case SelectPanel::MOVETOCAPCEL:
		MoveToCapcelUpdate();
		break;
	case SelectPanel::STAGECHAGE:

		break;
	}
}

void SelectPanel::Draw()
{
}

void SelectPanel::OnInspectorWindowDraw()
{
}

void SelectPanel::CopyComponent(IComponent* src)
{
}

void SelectPanel::TitleUpdate()
{
	if (Input::Key::Triggered(DIK_SPACE))
	{
		IsTitleToSelect = true;
	}

	if (IsTitleToSelect)
	{
		mTitleMoveTime++;
		float timeRite = mTitleMoveTime / mTitleMoveTimeMax;

		// スプライン曲線でカメラ座標と回転を変更
		mCameraObj->position = Vec3::Spline(mTitleCameraMovePos, timeRite);

		Vec3 rota = Vec3::Spline(mTitleCameraMoveRota, timeRite);
		mCameraObj->rotationE = DegreeToRadian(rota);

		mCameraObj->Update();

		// 移動が終了したら
		if (timeRite >= 1.0f)
		{
			mTitleMoveTime = 0;
			IsTitleToSelect = false;
			mSelectState = SELECTSTAGE;
			ConsoleWindow::Log("タイトルカメラ移動終了");
		}
	}
}

void SelectPanel::SelectStageUpdate()
{
	if (Input::Key::Triggered(DIK_A))
	{
		if (mSelectCurrentNum.x > 0)
		{
			mSelectCurrentNum.x--;
		}
	}
	if (Input::Key::Triggered(DIK_D))
	{
		if (mSelectCurrentNum.x < mSelectMaxNumX[(int)mSelectCurrentNum.y] - 1)
		{
			mSelectCurrentNum.x++;
		}
	}

	if (Input::Key::Triggered(DIK_W))
	{
		if (mSelectCurrentNum.y > 0)
		{
			mSelectCurrentNum.y--;

			CheckNumOver();
		}
	}
	if (Input::Key::Triggered(DIK_S))
	{
		if (mSelectCurrentNum.y < mSelectMaxNumY - 1)
		{
			mSelectCurrentNum.y++;

			CheckNumOver();
		}
	}

	if (mStageNum[(int32_t)mSelectCurrentNum.y][(int32_t)mSelectCurrentNum.x].state != PRESSED)
	{
		mStageNum[(int32_t)mSelectCurrentNum.y][(int32_t)mSelectCurrentNum.x].state = SELECT;

		if (Input::Key::Triggered(DIK_Z))
		{
			mStageNum[(int32_t)mSelectCurrentNum.y][(int32_t)mSelectCurrentNum.x].state = PRESSED;
		}
	}
	

	ConsoleWindow::Log(std::format("ステージ現在番号：Y,{} X,{}", mSelectCurrentNum.y, mSelectCurrentNum.x));

	for (size_t i = 0; i < mSelectMaxNumY; i++)
	{
		for (size_t j = 0; j < mSelectMaxNumX[i]; j++)
		{
			if (i != mSelectCurrentNum.y ||
				j != mSelectCurrentNum.x)
			{
				mStageNum[i][j].state = DISABLED;
			}

			switch (mStageNum[i][j].state)
			{
			case DISABLED:
				mStageNum[i][j].buttonObj->scale = mDisabledPanelScale;
				break;
			case SELECT:
				mStageNum[i][j].buttonObj->scale = mSelectPanelScale;

				break;
			case PRESSED:
				mStageNum[i][j].buttonObj->scale = mPressedPanelScale;
				mSelectState = MOVETOCAPCEL;
				break;
			}

			mStageNum[i][j].buttonObj->Update();
		}
	}

	ConsoleWindow::Log("ステージ番号オブジェを更新");
}

void SelectPanel::MoveToCapcelUpdate()
{
	mSceneChangeCameraTime++;
	float timeRite = mSceneChangeCameraTime / mSceneChangeCameraTimeMax;

	// スプライン曲線でカメラ座標と回転を変更
	mCameraObj->position = Vec3::Spline(mSceneChangeCameraMovePos, timeRite);

	Vec3 rota = Vec3::Spline(mSceneChangeCameraMoveRota, timeRite);
	mCameraObj->rotationE = DegreeToRadian(rota);

	mCameraObj->Update();

	// 移動が終了したら
	if (timeRite >= 1.0f)
	{
		mSceneChangeCameraTime = 0;
		mSelectState = STAGECHAGE;
		ConsoleWindow::Log("シーンチェンジカメラ移動終了");
	}
}

void SelectPanel::StageChangeUpdate()
{
}

void SelectPanel::CheckNumOver()
{
	// 上下した時のステージのサイズを調整
	if ((mSelectCurrentNum.x + 1) > mSelectMaxNumX[(int)mSelectCurrentNum.y])
	{
		mSelectCurrentNum.x = (float)(mSelectMaxNumX[(int)mSelectCurrentNum.y] - 1);
	}
}

void SelectPanel::InitTex()
{

}

RegisterScriptBody(SelectPanel);
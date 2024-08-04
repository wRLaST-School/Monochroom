#include "SelectPanel.h"
#include <ScriptComponent.h>
#include <SpDS.h>
#include <Util.h>
#include <Input.h>
#include <SceneManager.h>
#include <ConsoleWindow.h>
#include <SpTextureManager.h>
#include "AppOperationCommand.h"

void SelectPanel::Init()
{
	mSelectMaxNumY = 2;
	mSelectMaxNumX.resize(mSelectMaxNumY);

	mSelectMaxNumX[Stage1] = 5;
	mSelectMaxNumX[Stage2] = 5;

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

	mCapsuleDefuY = -2.6f;
	mCapsuleEndY = 0;

	mTitleCameraFirstPos = Vec3(-4.3f, 4.48f, 2.75f);
	mTitleCameraFirstRota = { 0, -10, 0 };
	mTitleCameraFirstRota = DegreeToRadianVec3(mTitleCameraFirstRota);

	mSceneChangeCameraFirstPos = Vec3(1.6f, 1.1f, -2.73f);
	mSceneChangeCameraFirstRota = Vec3(65, 0, 0);
	mSceneChangeCameraFirstRota = DegreeToRadianVec3(mSceneChangeCameraFirstRota);

	// カメラのセット
	mCameraObj = SceneManager::FindObject<Object3D>("Camera");

	mCameraObj->position = mTitleCameraFirstPos;
	mCameraObj->rotationE = mTitleCameraFirstRota;
	mCameraObj->Update();

	Camera::Set(*mCameraObj->CastTo<Camera>());

	// ゴーグルのセット
	mGoggleObj = SceneManager::FindObject<Object3D>("Goggle");
	mGoggleObj->brightnessCB.contents->w = 0;

	mLeftLensObj = SceneManager::FindObject<Object3D>("LeftLens");

	// カプセルのセット
	mCapsuleObj = SceneManager::FindObject<Object3D>("CapsuleDoor");
	mCapsuleObj->position.y = mCapsuleDefuY;

	// タイトルの文字のセット
	mTitleTextObj = SceneManager::FindObject<Object3D>("TitleText");
	mTitleTextObj->brightnessCB.contents->w = 0;

	// タイトルのロゴのセット
	mTitleRogoObj = SceneManager::FindObject<Object3D>("FirstScreen");
	mTitleRogoObj->brightnessCB.contents->w = 0;

	mTitleTextSinDefuPosY = 1.3f;
	mTitleTextSinSwingPosY = 0.02f;

	mGoggleSinDefuPosY = 0.8f;
	mGoggleSinSwingPosY = 0.05f;

	mGoggleSinDefuRotaZ = 0;
	mGoggleSinSwingRotaZ = 15;
	mGoggleSinSwingRotaZ = DegreeToRadian(mGoggleSinSwingRotaZ);



	mTitleSinTimer = 0;
	mTitleSinTimeMax = 180;

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

	mTitleCameraMoveRota.push_back(Vec3(0, -10, 0));
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
	mTitleState = ROGO;

	mTitleMoveTime = 0;
	mTitleMoveTimeMax = 60 * 6;

	mSceneChangeCameraTime = 0;
	mSceneChangeCameraTimeMax = 60 * 5;

	mEaseCapsule.SetEaseTimer(60 * 3);
	mEaseAlpha.SetEaseTimer((int)(60 * 1.5f));

	mButtonDisabledColor = { 160,55,55 };
	mButtonSelectColor = { 255,80,80 };

	mTextDisabledColor = { 200,130,130 };
	mTextSelectColor = { 255,219,219 };

#ifdef _DEBUG
	mEaseCapsule.SetEaseTimer(30);
	mEaseAlpha.SetEaseTimer((int)(30));
	mTitleMoveTimeMax = 30;
	mSceneChangeCameraTimeMax = 30;
#endif
}

void SelectPanel::Update()
{
	if (Input::Key::Triggered(DIK_R))
	{
		Reset();
	}

	//mTitleSinTimer++;
	//if (mTitleSinTimer >= mTitleSinTimeMax)
	//{
	//	mTitleSinTimer = 0;
	//}

	//mTitleTextObj->position.y = Sin_ZeroToOne(mTitleTextSinDefuPosY, mTitleSinTimeMax, mTitleSinTimer, mTitleTextSinSwingPosY);

	//mGoggleObj->position.y = Sin_ZeroToOne(mGoggleSinDefuPosY, mTitleSinTimeMax, mTitleSinTimer, mGoggleSinSwingPosY);
	//mGoggleObj->rotationE.y += DegreeToRadian(2.0f);
	//mGoggleObj->rotationE.x = Sin_ZeroToOne(mGoggleSinDefuRotaZ, mTitleSinTimeMax, mTitleSinTimer, mGoggleSinSwingRotaZ);

	mTitleTextObj->Update();
	mGoggleObj->Update();

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
		StageChangeUpdate();
		break;
	}
	ConsoleWindow::Log(mStageName);
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

bool SelectPanel::GetIsChangeScene()
{
	return IsChangeStage;
}

void SelectPanel::SetIsChangeScene(bool IsChage)
{
	IsChangeStage = IsChage;
}

std::string SelectPanel::GetStageName()
{
	return mStageName;
}

float SelectPanel::DegreeToRadian(float angle)
{
	float radian = angle * (float)(PI / 180);

	return radian;
}

Vec3 SelectPanel::DegreeToRadianVec3(Vec3& angle)
{
	Vec3 radian = angle * (float)(PI / 180);

	return radian;
}

float  SelectPanel::Sin_ZeroToOne(float defuValue, float maxCount, float nowCount, float swingWidth)
{
	float result = defuValue + swingWidth * (sin((float)PI * 2 / maxCount * nowCount));
	return result;
}

Vec3 SelectPanel::Sin_ZeroToOne(Vec3 defuValue, float maxCount, float nowCount, Vec3 swingWidth)
{
	float x = defuValue.x + sin((float)PI * 2 / maxCount * nowCount) * swingWidth.x;
	float y = defuValue.y + sin((float)PI * 2 / maxCount * nowCount) * swingWidth.y;
	float z = defuValue.z + sin((float)PI * 2 / maxCount * nowCount) * swingWidth.z;

	return Vec3(x, y, z);
}

void SelectPanel::TitleUpdate()
{
	switch (mTitleState)
	{
	case SelectPanel::ROGO:

		if (!IsAlphaOn)
		{
			mEaseAlpha.Update();
			mTitleRogoObj->brightnessCB.contents->w = mEaseAlpha.In(0, 1);

			if (mEaseAlpha.GetisEnd())
			{
				mEaseAlpha.Reset();
				IsAlphaOn = true;
			}
		}
		else
		{
			mEaseAlpha.Update();
			mTitleRogoObj->brightnessCB.contents->w = mEaseAlpha.In(1, 0);
			if (mEaseAlpha.GetisEnd())
			{
				mEaseAlpha.Reset();
				IsAlphaOn = false;
				mTitleState = NORMAL;
			}
		}


		break;
	case SelectPanel::NORMAL:
		if (!IsAlphaOn)
		{
			mEaseAlpha.Update();
			float alpha = mEaseAlpha.In(0, 1);
			mGoggleObj->brightnessCB.contents->w = alpha;
			mTitleTextObj->brightnessCB.contents->w = alpha;
			mLeftLensObj->brightnessCB.contents->w = alpha;

			if (mEaseAlpha.GetisEnd())
			{
				mEaseAlpha.Reset();
				IsAlphaOn = true;
			}
		}

		if (AppOperationCommand::GetInstance()->UIDicisionCommand())
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
			mCameraObj->rotationE = DegreeToRadianVec3(rota);

			mCameraObj->Update();

			// 移動が終了したら
			if (timeRite >= 1.0f)
			{
				mTitleMoveTime = 0;
				IsTitleToSelect = false;
				IsAlphaOn = false;
				mSelectState = SELECTSTAGE;
				ConsoleWindow::Log("タイトルカメラ移動終了");
			}
		}

		break;
	}

	mTitleRogoObj->Update();
	mGoggleObj->Update();
}

void SelectPanel::SelectStageUpdate()
{
	if (AppOperationCommand::GetInstance()->UISelectLeftCommand())
	{
		if (mSelectCurrentNum.x > 0)
		{
			mSelectCurrentNum.x--;
		}
	}
	if (AppOperationCommand::GetInstance()->UISelectRightCommand())
	{
		if (mSelectCurrentNum.x < mSelectMaxNumX[(int)mSelectCurrentNum.y] - 1)
		{
			mSelectCurrentNum.x++;
		}
	}

	if (AppOperationCommand::GetInstance()->UISelectUpCommand())
	{
		if (mSelectCurrentNum.y > 0)
		{
			mSelectCurrentNum.y--;

			CheckNumOver();
		}
	}
	if (AppOperationCommand::GetInstance()->UISelectDownCommand())
	{
		if (mSelectCurrentNum.y < mSelectMaxNumY - 1)
		{
			mSelectCurrentNum.y++;

			CheckNumOver();
		}
	}

	if (AppOperationCommand::GetInstance()->UISelectDownCommand() ||
		AppOperationCommand::GetInstance()->UISelectUpCommand() ||
		AppOperationCommand::GetInstance()->UISelectLeftCommand() ||
		AppOperationCommand::GetInstance()->UISelectRightCommand())
	{
		SoundManager::Play("Select");
	}

	if (mStageNum[(int32_t)mSelectCurrentNum.y][(int32_t)mSelectCurrentNum.x].state != PRESSED)
	{
		mStageNum[(int32_t)mSelectCurrentNum.y][(int32_t)mSelectCurrentNum.x].state = SELECT;

		if (AppOperationCommand::GetInstance()->UIDicisionCommand())
		{
			mStageNum[(int32_t)mSelectCurrentNum.y][(int32_t)mSelectCurrentNum.x].state = PRESSED;
			SoundManager::Play("Decide");
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

				*mStageNum[i][j].buttonObj->brightnessCB.contents = mButtonDisabledColor;
				*mStageNum[i][j].hyphenObj->brightnessCB.contents = mTextDisabledColor;
				*mStageNum[i][j].stageBaseNumObj->brightnessCB.contents = mTextDisabledColor;
				*mStageNum[i][j].stageSubNumObj->brightnessCB.contents = mTextDisabledColor;

				break;
			case SELECT:
				mStageNum[i][j].buttonObj->scale = mSelectPanelScale;

				*mStageNum[i][j].buttonObj->brightnessCB.contents = mButtonSelectColor;
				*mStageNum[i][j].hyphenObj->brightnessCB.contents = mTextSelectColor;
				*mStageNum[i][j].stageBaseNumObj->brightnessCB.contents = mTextSelectColor;
				*mStageNum[i][j].stageSubNumObj->brightnessCB.contents = mTextSelectColor;

				break;
			case PRESSED:
				std::string stageDirectory="Assets/Scene/Stage/";
				std::string stageNum= "Stage" + std::to_string(i + 1) + "_" + std::to_string(j + 1);
				std::string stageExtensionName = ".scene";
				mStageName = stageDirectory + stageNum + stageExtensionName;

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
	mCameraObj->rotationE = DegreeToRadianVec3(rota);

	mCameraObj->Update();

	// 移動が終了したら
	if (timeRite >= 1.0f)
	{
		mSceneChangeCameraTime = 0;
		mSelectState = STAGECHAGE;
		ConsoleWindow::Log("シーンチェンジカメラ移動終了");

		SoundManager::Play("Blink");
	}
}

void SelectPanel::StageChangeUpdate()
{
	
	mEaseCapsule.Update();

	// カプセルのオブジェクトを上げて、ステージに移動
	mCapsuleObj->position.y = mEaseCapsule.In(mCapsuleDefuY, mCapsuleEndY);
	mCapsuleObj->Update();

	// 終了したらステージに移動
	if (mEaseCapsule.GetisEnd())
	{
		IsChangeStage = true;
	}
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

void SelectPanel::Reset()
{
	mTitleState = ROGO;
	mSelectState = TITLE;

	mStageNum[(int32_t)mSelectCurrentNum.y][(int32_t)mSelectCurrentNum.x].state = DISABLED;

	mSelectCurrentNum = { 0,0 };

	mCameraObj->position = mTitleCameraFirstPos;
	mCameraObj->rotationE = mTitleCameraFirstRota;
	mCameraObj->Update();

	IsAlphaOn = false;
	IsTitleToSelect = false;

	mTitleSinTimer = 0;

	mTitleMoveTime = 0;
	mSceneChangeCameraTime = 0;

	mEaseCapsule.Reset();
	mEaseAlpha.Reset();

	mGoggleObj->brightnessCB.contents->w = 0;
	mTitleTextObj->brightnessCB.contents->w = 0;
	mTitleRogoObj->brightnessCB.contents->w = 0;

	mCapsuleObj->position.y = mCapsuleDefuY;

	mGoggleObj->Update();
	mTitleTextObj->Update();
	mTitleRogoObj->Update();
	mCapsuleObj->Update();
}



RegisterScriptBody(SelectPanel);
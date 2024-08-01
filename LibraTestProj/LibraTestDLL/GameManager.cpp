#include "GameManager.h"
#include <SceneFromFile.h>
#include <SceneManager.h>
#include <ConsoleWindow.h>
#include <AppOperationCommand.h>
#include <StageGenerating.h>
#include <BlinkTransition.h>
#include <Input.h>

void GameManager::Awake()
{
	mPlayer = SceneManager::FindObject<Object3D>("Player");
	mCamera = SceneManager::FindObject<Camera>("Camera");
	mStageGenerater = SceneManager::FindObjectWithTag<StageGenerater>("StageGenerater");
	mSelectPanel = SceneManager::FindObject<SelectPanel>("SelectScript");
	mUIScript = SceneManager::FindObject<SuperUI>("UIScript");

	isStop = false;

	if (SceneManager::GetCurrentScene()->GetName() == "Title" ||
		SceneManager::GetCurrentScene()->GetName() == "StageSelect")
	{
		StageGenerating::info.isDraw = false;
		//BlinkTransition::mIsChangeScene = false;
		BlinkTransition::Reset();
	}
	else
	{
		StageGenerating::info.isDraw = true;
	}
}

void GameManager::Init()
{
}

void GameManager::Update()
{
	if (AppOperationCommand::GetInstance()->UserDebugStopGameCommand())
	{
		isStop = isStop ? false : true;
	}

	ConsoleWindow::Log(std::format("GameManager::isStop : {}", isStop));

	if (!SceneManager::GetCurrentScene())
	{
		ConsoleWindow::Log("SceneManager::GetCurrentScene is null");

		return;
	}

	if (SceneManager::GetCurrentScene()->GetName() == "Title")
	{
		if (AppOperationCommand::GetInstance()->PlayerConfirmCommand())
		{

			//// シーンの切り替え処理
			//SceneManager::LoadScene<SceneFromFile>("Assets/Scene/Game.scene");
			//SceneManager::WaitForLoadAndTransition();

			OutputDebugStringA("SceneChangeClickToGame\n");

			if (!BlinkTransition::mIsChangeScene)
			{
				BlinkTransition::Start();
				BlinkTransition::mIsChangeScene = true;
			}
		}

		if (BlinkTransition::info.isInEnd)
		{
			// シーンの切り替え処理
			SceneManager::LoadScene<SceneFromFile>("Assets/Scene/Game.scene");
			SceneManager::WaitForLoadAndTransition();
		}
	}
	else if (SceneManager::GetCurrentScene()->GetName() == "StageSelect")
	{
		ConsoleWindow::Log("Select Is Set");
		if (!mSelectPanel)
		{
			ConsoleWindow::Log("mSelectPanel is null");
			return;
		}
		if (mSelectPanel->GetIsChangeScene())
		{
			//// シーンの切り替え処理
			//SceneManager::LoadScene<SceneFromFile>("Assets/Scene/Game.scene");
			//SceneManager::WaitForLoadAndTransition();

			OutputDebugStringA("SceneChangeClickSelectToGame\n");

			if (!BlinkTransition::mIsChangeScene)
			{
				BlinkTransition::Start();
				BlinkTransition::mIsChangeScene = true;
			}
		}

		if (BlinkTransition::mIsChangeScene)
		{
			if (BlinkTransition::info.isInEnd)
			{
				// シーンの切り替え処理
				SceneManager::LoadScene<SceneFromFile>(mSelectPanel->GetStageName());
				SceneManager::WaitForLoadAndTransition();
			}
		}
	}
	else
	{
		ConsoleWindow::Log("Game Is Set");
		if (!mUIScript)
		{
			ConsoleWindow::Log("UI is null");
			return;
		}
		// UIでタイトルにもどる時
		if (mUIScript->GetBackToTitle())
		{
			OutputDebugStringA("SceneChangeClickToSelectScene\n");

			if (!BlinkTransition::mIsChangeScene)
			{
				BlinkTransition::mToTitle = true;
				BlinkTransition::Start();
				BlinkTransition::mIsChangeScene = true;
			}
		}

		if (BlinkTransition::mIsChangeScene)
		{
			if (BlinkTransition::info.isInEnd)
			{
				if (BlinkTransition::mToTitle)
				{
					// シーンの切り替え処理
					SceneManager::LoadScene<SceneFromFile>("Assets/Scene/StageSelect.scene");
					SceneManager::WaitForLoadAndTransition();
				}
				else
				{
					// リセット
					std::string sceneName = SceneManager::GetCurrentScene()->GetName();
					if (AppOperationCommand::GetInstance()->ReStartCommand())
					{
						// シーンの切り替え処理
						SceneManager::LoadScene<SceneFromFile>("Assets/Scene/" + sceneName + ".scene");
						SceneManager::WaitForLoadAndTransition();
					}
				}
			}
		}

		if (!mStageGenerater)
		{
			ConsoleWindow::Log("mStageGenerater is null");
			return;
		}

		if (isStop == false)
		{
			Input::Mouse::SetPos(Float2(960, 540));
			Input::Mouse::HideCursor();
		}
		else
		{
			Input::Mouse::ShowCursorM();
		}

		// ステージ生成
		mStageGenerater->Start();
	}

	ConsoleWindow::Log("GameManager: isActive");
	BlinkTransition::TransitionIn();

	// ロード終わった
	if (SceneManager::transitionQueued)
	{
		BlinkTransition::mIsLoaded = true;
	}

	if (BlinkTransition::mIsChangeScene &&
		BlinkTransition::mIsLoaded)
	{
		BlinkTransition::TransitionOut();

		if (BlinkTransition::info.isOutEnd)
		{
			BlinkTransition::mIsChangeScene = false;
			BlinkTransition::mIsLoaded = false;
		}
	}
}

void GameManager::Draw()
{

}

void GameManager::CopyComponent(IComponent* src)
{

}

Object3D* GameManager::GetPlayer()
{
	return mPlayer;
}

Camera* GameManager::GetCamera()
{
	return mCamera;
}

StageGenerater* GameManager::GetStageGenerater()
{
	return mStageGenerater;
}

bool GameManager::GetisStop()
{
	return isStop;
}

void GameManager::SetIsStop(bool IsStop)
{
	isStop = IsStop;
}

GameManager* GameManager::GetInstance()
{
	GameManager* instance = SceneManager::FindObject<GameManager>("GameManager");
	return instance;
}

RegisterScriptBody(GameManager);
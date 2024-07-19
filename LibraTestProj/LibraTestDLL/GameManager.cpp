#include "GameManager.h"
#include <SceneFromFile.h>
#include <SceneManager.h>
#include <ConsoleWindow.h>
#include <AppOperationCommand.h>
#include <StageGenerating.h>
#include <BlinkTransition.h>
#include <Input.h>

bool GameManager::mIsChangeScene = false;

void GameManager::Awake()
{
	mPlayer = SceneManager::FindObject<Object3D>("Player");
	mCamera = SceneManager::FindObject<Camera>("Camera");
	mStageGenerater = SceneManager::FindObjectWithTag<StageGenerater>("StageGenerater");
	isStop = false;

	if (SceneManager::GetCurrentScene()->GetName() == "Title")
	{
		StageGenerating::info.isDraw = false;
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

			if (!mIsChangeScene)
			{
				BlinkTransition::Start();
				mIsChangeScene = true;
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

	}
	else
	{
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

		// リセット
		std::string sceneName = SceneManager::GetCurrentScene()->GetName();
		if (AppOperationCommand::GetInstance()->ReStartCommand())
		{
			// シーンの切り替え処理
			SceneManager::LoadScene<SceneFromFile>("Assets/Scene/" + sceneName + ".scene");
			SceneManager::WaitForLoadAndTransition();
		}
	}

	ConsoleWindow::Log("GameManager: isActive");
	BlinkTransition::TransitionIn();
	if (SceneManager::GetLoadState() == SceneManager::LoadState::NotInProgress)
	{
		if (mIsChangeScene == true)
		{
			BlinkTransition::TransitionOut();

			if (BlinkTransition::info.isOutEnd)
			{
				mIsChangeScene = false;
			}
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
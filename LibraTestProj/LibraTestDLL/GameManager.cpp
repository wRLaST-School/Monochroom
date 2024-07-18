#include "GameManager.h"
#include <SceneFromFile.h>
#include <SceneManager.h>
#include <ConsoleWindow.h>
#include <AppOperationCommand.h>
#include <StageGenerating.h>
#include <BlinkTransition.h>

void GameManager::Init()
{
	mPlayer = SceneManager::FindObject<Object3D>("Player");
	mCamera = SceneManager::FindObject<Camera>("Camera");
	isStop = false;

	if (SceneManager::GetCurrentScene()->GetName() == "Title")
	{
		StageGenerating::info.isDraw = false;
	}
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
			if (BlinkTransition::info.isInEnd)
			{
				BlinkTransition::Start();
			}
		}

		if (BlinkTransition::info.isInEnd)
		{
			// シーンの切り替え処理
			SceneManager::LoadScene<SceneFromFile>("Assets/Scene/Game.scene");
			SceneManager::WaitForLoadAndTransition();
		}
	}

	std::string sceneName = SceneManager::GetCurrentScene()->GetName();
	ConsoleWindow::Log(sceneName);
	if (AppOperationCommand::GetInstance()->ReStartCommand())
	{
		// シーンの切り替え処理
		SceneManager::LoadScene<SceneFromFile>("Assets/Scene/" + sceneName + ".scene");
		SceneManager::WaitForLoadAndTransition();
	}

	BlinkTransition::TransitionIn();

	// シーン切り替え終わった時にOut
	//BlinkTransition::TransitionOut();
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
#include "GameManager.h"
#include <SceneManager.h>
#include <ConsoleWindow.h>
#include <AppOperationCommand.h>

void GameManager::Init()
{
	mPlayer = SceneManager::FindObject<Object3D>("Player");
	isStop = false;
}

void GameManager::Update()
{
	if (AppOperationCommand::GetInstance()->UserDebugStopGameCommand())
	{
		isStop = isStop ? false : true;
	}

	ConsoleWindow::Log(std::format("GameManager::isStop : {}", isStop));

	if (SceneManager::currentScene->GetName() == "Title")
	{
		if (AppOperationCommand::GetInstance()->PlayerConfirmCommand())
		{
			// シーンの切り替え処理

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
	static GameManager* instance = SceneManager::FindObject<GameManager>("GameManager");;
	return instance;
}

RegisterScriptBody(GameManager);